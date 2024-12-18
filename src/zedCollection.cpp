 // Standard includes
#include <string.h>

// OpenCV include (for display)
#include <opencv2/opencv.hpp>

#include <sl/Camera.hpp>

int getOCVtype(sl::MAT_TYPE type) {
    int cv_type = -1;
    switch (type) {
        case sl::MAT_TYPE::F32_C1: cv_type = CV_32FC1; break;
        case sl::MAT_TYPE::F32_C2: cv_type = CV_32FC2; break;
        case sl::MAT_TYPE::F32_C3: cv_type = CV_32FC3; break;
        case sl::MAT_TYPE::F32_C4: cv_type = CV_32FC4; break;
        case sl::MAT_TYPE::U8_C1: cv_type = CV_8UC1; break;
        case sl::MAT_TYPE::U8_C2: cv_type = CV_8UC2; break;
        case sl::MAT_TYPE::U8_C3: cv_type = CV_8UC3; break;
        case sl::MAT_TYPE::U8_C4: cv_type = CV_8UC4; break;
        default: break;
    }
    return cv_type;
}

/**
* Conversion function between sl::Mat and cv::Mat
**/
cv::Mat slMat2cvMat(sl::Mat& input) {
    // Since cv::Mat data requires a uchar* pointer, we get the uchar1 pointer from sl::Mat (getPtr<T>())
    // cv::Mat and sl::Mat will share a single memory structure
    return cv::Mat(input.getHeight(), input.getWidth(), getOCVtype(input.getDataType()), input.getPtr<sl::uchar1>(sl::MEM::CPU), input.getStepBytes(sl::MEM::CPU));
}

int main(int argc, char **argv) {   

    sl::Camera zed;
    sl::InitParameters params;
    params.camera_resolution = sl::RESOLUTION::HD1200;
    params.camera_fps = 60;
    zed.open(params);

    // create a file to save images names
    std::vector<std::string> v_names;

    sl::Mat imZL, imZR;
    cv::Mat imL, imR;

    int nb_save = 0;
    const int NB_REQUIRED = 20;
    while (nb_save < NB_REQUIRED) {

        if (zed.grab() != sl::ERROR_CODE::SUCCESS) {
            printf("Failed to grab zed\n");
        }

        zed.retrieveImage(imZL, sl::VIEW::LEFT);
        zed.retrieveImage(imZR, sl::VIEW::RIGHT);

        imL = slMat2cvMat(imZL);
        imR = slMat2cvMat(imZR);

        // Left and Right mat are directly updated because they are ref.

        cv::imshow("Left", imL); // display left image
        auto k = cv::waitKey(30);

        // if Space-bar is pressed, save the image
        if (k == 32) {
            std::string im_name("zed_image_L" + std::to_string(nb_save) + ".png");
            cv::imwrite(im_name, imL);
            v_names.push_back(im_name);

            im_name = "zed_image_R" + std::to_string(nb_save) + ".png";
            cv::imwrite(im_name, imR);
            v_names.push_back(im_name);

            nb_save++;
            std::cout << "Save im " << nb_save << "/" << NB_REQUIRED << std::endl;
        }
    }
    // close file and camera
    cv::FileStorage fs("zed_image_list.xml", cv::FileStorage::WRITE);
    fs.write("images", v_names);
    fs.release();
    zed.close();
    return EXIT_SUCCESS;
}
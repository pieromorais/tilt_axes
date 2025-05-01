// TODO: change from float to double precision
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "opencv4/opencv2/opencv.hpp"

using namespace cv;

struct boudingbox {
        // these number need to be normalized according to the image size

	int class_id;
	float center_x;
	float center_y;
	float width;
	float height;
};

void tilt_by_theta_text(boudingbox& box, float theta, cv::Point2f center) {
	// tilt the bounding box by theta degrees
	// this function is not implemented yet
	// you can use the following formula to tilt the bounding box
	// NOTE: minus sign is used to rotate the box in the opposite direction,
	// because the y axis is inverted in the image, moving from top to bottom
	float theta_radians =  - theta * M_PI / 180.0; // convert degrees to radians
	
	// print box center x and y 
	//std::cout << "Box center x: " << box.center_x << ", Box center y: " << box.center_y << std::endl;

	// translate the box to the center of the image
	box.center_x -= center.x;
	box.center_y -= center.y;

	// print box center x and y after translation
	//std::cout << "Box center x after translation: " << box.center_x << ", Box center y after translation: " << box.center_y << std::endl;
	
	box.center_x = box.center_x * cos(theta_radians) - box.center_y * sin(theta_radians);
	box.center_y = box.center_x * sin(theta_radians) + box.center_y * cos(theta_radians);

	// print box center x and y after rotation
	//std::cout << "Box center x after rotation: " << box.center_x << ", Box center y after rotation: " << box.center_y << std::endl;

	// translate the box back to the original position
	box.center_x += center.x;
	box.center_y += center.y;

	// print box center x and y after translation
	// std::cout << "Box center x after translation: " << box.center_x << ", Box center y after translation: " << box.center_y << std::endl;

	// round the values to the nearest integer
	box.center_x = std::round(box.center_x);
	box.center_y = std::round(box.center_y);

	// print box center x and y after rounding
	//std::cout << "Box center x after rounding: " << box.center_x << ", Box center y after rounding: " << box.center_y << std::endl;
}

void tilt_by_theta_image(cv::Mat& image, float theta) {
	// tilt the image by theta degrees
	// this function is not implemented yet
	// you can use the following formula to tilt the image
	float theta_radians = theta * M_PI / 180.0; // convert degrees to radians
	cv::Mat rotation_matrix = cv::getRotationMatrix2D(cv::Point2f(image.cols / 2, image.rows / 2), theta, 1);
	cv::warpAffine(image, image, rotation_matrix, image.size());
}

void draw_bounding_box(cv::Mat& image, const std::vector<boudingbox>& boxes) {
	// draw the bounding boxes on the image
	for (const auto& box : boxes) {
		cv::rectangle(image, cv::Point(box.center_x - box.width / 2, box.center_y - box.height / 2),
			cv::Point(box.center_x + box.width / 2, box.center_y + box.height / 2),
			cv::Scalar(0, 255, 0), 2);
	}
}

void renormalize_bounding_box(boudingbox& box, int image_width, int image_height) {
	// renormalize the bounding box according to the image size
	box.center_x /= image_width;
	box.center_y /= image_height;
	box.width /= image_width;
	box.height /= image_height;
}

std::string is_theta_negative(float theta) {
	// TODO: maybe change the nomenclature to use clockwise and counterclockwise instead of neg and pos
	// check if the angle is negative
    
	int theta_int = static_cast<int>(theta);
	if (theta < 0) {
	    // convert to integer
	    return "neg" + std::to_string(-theta_int);
	}else {
	    return "pos" + std::to_string(theta_int);
	}
}

int main(int argc, char** argv) {


    if (argc < 4) {
	std::cerr << "Usage: " << argv[0] << " <image>" << " <txt file>" << " <angle degrees>"<< std::endl;
	return 1;
    }

    // read the image file
    cv::Mat image = cv::imread(argv[1]);
    if (image.empty()) {
	std::cerr << "Error opening image: " << argv[1] << std::endl;
	return 1;
	}

    const int image_width = image.cols; 
    const int image_height = image.rows;

    // get image center pixel
    //NOTE: number is round to nearest integer in the function tilt_by_theta_text
    cv::Point2f center(image_width / 2, image_height / 2);

    
    // tilt image by theta degrees
    int theta = std::atoi(argv[3]); // set the angle of rotation
    tilt_by_theta_image(image, theta);
    
       // open the text file
    std::ifstream file(argv[2]);
    if (!file.is_open()) {
	std::cerr << "Error opening file: " << argv[1] << std::endl;
	return 1;
    }

    // read txt file and store data in a vector of boudingbox's structs
    // the txt file is in the format:
    // class_id center_x center_y width height
    // where class_id is an integer and center_x, center_y, width, height are floats
    // the values are separated by spaces
    std::vector<boudingbox> boxes;
    boudingbox box;

    while (file >> box.class_id >> box.center_x >> box.center_y >> box.width >> box.height) {

	box.center_x *= image_width; // denormalize	
	box.center_y *= image_height; // denormalize
	box.width *= image_width; // denormalize
	box.height *= image_height; // denormalize
	
	// NOTE: width and height can be negative values, therefore out of the scope of the image
	// in this case, we need to check if the box is out of the image scope and make sure to create a new bouding box
	// smaller than before, w/ lower limit greater than 0. Similar thoght for bouding boxes out of the scope for being greater than the image size
	//
	// NOTE: width and height can be float values, which does not make sense for an image. Need to test how to round them.
	
	// tilt image by theta degrees
	int theta = std::atoi(argv[3]); // set the angle of rotation
	tilt_by_theta_text(box, theta, center);

	boxes.push_back(box);
    }

    // print the data
//    for (const auto& box : boxes) {
//		std::cout << "Class ID: " << box.class_id << ", Center X: " << box.center_x
//				  << ", Center Y: " << box.center_y << ", Width: " << box.width
//				  << ", Height: " << box.height << std::endl;
//	}
     // make window resizable and with scrollbar
    
    // draw the bounding boxes on the image
    draw_bounding_box(image, boxes);
    
    //cv::namedWindow("Image", cv::WINDOW_NORMAL);
    //cv::resizeWindow("Image", 800, 600);
    //show image
    //cv::imshow("Image", image);

    // image name
    std::string image_name = argv[1];
    // strip extension from image name
    std::string::size_type pos = image_name.find_last_of(".");
    if (pos == std::string::npos) {
	std::cerr << "Error: no extension found in image name" << std::endl;
	return 1;
	}

    //NOTE: remove redundant code below that it is used twice: once for saving the image and once for saving the txt file
    // get the image name without the extension
    std::string image_name_without_extension = image_name.substr(0, pos);
    // get extension
    std::string extension = image_name.substr(pos);

    // convert theta to string
    // check if angle is negative
    std::string theta_sign = is_theta_negative(theta);
    std::string theta_str = std::to_string(theta);
    std::string file_name_image = image_name_without_extension + "_" + theta_sign + ".jpg";


    // wait for a key press indefinitely
    // save image as jpeg in the output folder
    cv::imwrite(file_name_image, image);
    std::cout << "Image saved as: " << file_name_image << std::endl;
    cv::waitKey(0);




    // strip extension f
    // get the image name without the extension

    // convert theta to string
    // check if angle is negative
    std::string file_name_txt = image_name_without_extension + "_" + theta_sign + ".txt";

    // renormalize the bounding boxes according to the image size
    for (auto& box : boxes) {
	renormalize_bounding_box(box, image_width, image_height);
	}

    // write the bounding boxes to a new text file
    // the new file is in the format:
    // class_id center_x center_y width height - all normalized
    // the values are separated by spaces
    // the name of the file should be the same as the image file, but with a different extension
    // adding the angle of ratation in the name of the file after _ i.e. image_45.txt
    
    std::ofstream output_file(file_name_txt);
    if (!output_file.is_open()) {
	std::cerr << "Error opening file: " << file_name_txt << std::endl;
	return 1;
	}

	// write the data to the file
    for (const auto& box : boxes) {
	    output_file << box.class_id << " " << box.center_x << " " << box.center_y << " "
		    << box.width << " " << box.height << std::endl;
    }

    // close the file
    output_file.close();

    // print the name of the file
    std::cout << "File saved as: " << file_name_txt << std::endl;

// close the file
    file.close();
    return 0;
}

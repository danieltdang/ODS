# Object Management System (OMS)
Utilizes [OpenCV 4.8.0](https://github.com/opencv/opencv/releases/tag/4.8.0), in combination with the vision AI model [YOLOv5](https://github.com/ultralytics/yolov5) to process images to identify and categorize products in an inventory. Store product details, quantities, and images in a [MySQL](https://github.com/mysql/mysql-server) database to track inventory levels and facilitate restocking.
## Overview

Create an inventory management system using C++, OpenCV, and MySQL database integration. This application will use image processing to identify and categorize products within an inventory, while storing essential product details, quantities, and images in an SQL database. This system will enable efficient inventory tracking and streamline restocking processes.

## Project Direction

1. **Project Setup:**
   - Configure the development environment with C++, OpenCV, and a MySQL database library.
   - Create a new C++ project within the chosen IDE, Visual Studio 2022, in this case.

2. **Database Design:**
   - Design the structure of the MySQL database, including tables like `Products`, `Categories`, and `Inventory`.
   - Define the necessary fields for each table, such as `product_id`, `name`, `description`, `category_id`, `quantity`, and `image_path`.

3. **Image Processing:**
   - Utilize OpenCV to perform image processing tasks.
   - Implement techniques like object detection and segmentation to identify and categorize products.
   - Apply preprocessing methods to enhance image quality and reduce noise.

4. **Database Integration:**
   - Write C++ code to establish a connection to the MySQL database.
   - Develop functions to insert new products, update quantities, and retrieve product information from the database.
   - Manage image paths, ensuring consistency in storage and retrieval.

5. **User Interface:**
   - Create a user interface using a command-line interface (CLI) for interaction.

6. **Image Capture (Optional):**
   - Integrate camera modules or webcams to capture product images directly into the application.
   - Use OpenCV to preprocess captured images before storing them in the database.

7. **Product Categorization:**
   - Develop algorithms to categorize products based on image-extracted information.
   - Match features with existing product categories and update the database accordingly.

8. **Inventory Tracking:**
   - Implement logic for tracking inventory levels.
   - Update product quantities in the database as products are added or sold.
   - Set up notifications for low stock levels to facilitate restocking.

9. **Testing and Debugging:**
   - Thoroughly test the application with various products and images.
   - Debug and address any issues arising during testing, ensuring proper image processing and database functionality.

10. **Documentation and Deployment:**
    - Document the application code and its components, providing instructions for setup and usage.
    - Deploy the application in a testing environment, ensuring stability were the application to be deployed.

## Tech Stack

- **Programming Language:** C++
- **Image Processing:** OpenCV
- **Database:** MySQL
- **User Interface:** Command-line Interface (CLI)

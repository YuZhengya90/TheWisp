# Welcome to TheWisp!

TheWisp is a predict software for SAP Business One, there are 6 models in this software:

 1. Predict Purchase Price.
 2. Predict Sales Price.
 3. Predict Temperature.
 4. Predict Sales Quantity.
 5. Predict Profit.
 6. Operation Advice.

All these models are build with data(Purchase Order, Sales Order) of the past 7 years.(from 2011 to 2017).

# Folders

TheWisp stores your files in your browser, which means all your files are automatically saved locally and are accessible **offline!**

## ./resource
The main folder of this project. We use MFC to draw the main form. 

## ./data-mining
Files about the models, use Java to implement.

## ./lib
Some OpenGL libraries. We use OpenGL to draw show the predict data.

## ./jar
The release model jar files of data-mining.

# How to lunch this project

At first , you need to install VS2013, and then use VS2013 to open the project file TheWisp.sln
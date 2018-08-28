

# Welcome to Horus' data mining package!

This package provides simple data mining models for **SAP Business One**.

# Environment
This package uses **JDK 1.8.0** and **Weka 3.8.2**.

# Install

 - Download **weka.jar** and **horus.datamining.jar** from **./lib** to your Java project and add them into build paths.
 - Download all **.model** and **.arff** files from **./models** to a directory as you like. Remember this directory that may be used in your Java code.
 
 # API Guideline
 ## Basic Concepts
 
 - **Environment**: A data structure that contains configurations used to load models and data.
 
 - **Model**: An abstract data structure that is used to give suggestions according to the given feature vector.
 
 - **Feature Vector**: A serial of fields that compose a record for model to solve. Typically, the last field is the field for suggestion, others are for model to use to give the suggestion. Two kinds of fields are supported: **numeric and nominal (enumeration)**. The schema could be found in **.arff** files.
 
 - **Suggestion**: The output of a model solution. It may consists of several fields. Usually it consists of one field of the last one of corresponding feature vector.

## Wrapper
There is one corresponding wrapper class for each model in package **horus.datamining.wrapper**. These wrappers provide use of the models without creating model objects or building feature vectors. Only model path specifications and method calls with corresponding parameters are needed. The results are always double arrays. The input and output schemas are specified in the comments of the source code: **./src/horus/datamining/wrapper/***.

# Sample Code
You can refer to ./src/horus/datamining/Sample.java as coding example.

# Notice

 - The **StockQuantity** in feature vector of **SaleQuantityPrediction**
   is the stock quantity of yesterday plus the purchase quantity of
   today.
 - The **StockQuantity** in feature vector of **ProfitPrediction** and **OperationAdvice** is
   the stock quantity of yesterday.
 - In **ProfitPrediction** and **OperationAdvice**, **today** is the day that is going to sell
   mantous. **Target day** is the day whose profit is to be predicted.
   The sale/purchase price/quantity belong to **today**, not **target
   day**.


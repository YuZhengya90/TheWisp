package horus.datamining.wrapper;

import horus.datamining.env.Environment;
import horus.datamining.env.EnvironmentImpl;
import horus.datamining.model.*;
import horus.datamining.model.feature.FeatureVector;


public class SalePricePredictionWrapper
{
	private static Environment environment;
	private static Model model;


	public static void setModelPath(String modelPath) throws Exception
	{
		environment = new EnvironmentImpl();
		environment.setModelPath(modelPath);
		model = new SalePricePrediction(environment);
	}


	// Input: year - numeric, month - numeric, day - numeric	(in 2017)
	// Output: Price - numeric
	public static double[] predictPrice(int year, int month, int day)
	{
		FeatureVector featureVector = model.createFeatureVector();
		featureVector.setValue("Year", year);
		featureVector.setValue("Month", month);
		featureVector.setValue("Day", day);

		double[] result = null;
		try
		{
			Suggestion suggestion = model.solve(featureVector);
			result = new double[1];
			result[0] = (double) suggestion.getFieldValue("Price");
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}


	public static void test() throws Exception
	{
		SalePricePredictionWrapper.setModelPath("D:/my-git/data-mining/DataMining/models/");
		double[] result = SalePricePredictionWrapper.predictPrice(2017, 4, 1);
		System.out.println(result[0]);
	}
}

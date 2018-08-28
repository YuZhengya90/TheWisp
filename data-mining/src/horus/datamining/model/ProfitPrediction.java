package horus.datamining.model;

import horus.datamining.env.Environment;
import weka.core.Attribute;
import weka.core.Instance;
import weka.core.Instances;


public class ProfitPrediction extends AbstractLearningModel
{

	public ProfitPrediction(Environment environment) throws Exception
	{
		super(environment);
	}


	@Override
	protected String getName()
	{
		return "ProfitPrediction";
	}

	
	@Override
	protected Suggestion solveInstances(Instances instances)
	{
		Suggestion suggestion = null;
		Instance instance = instances.get(0);
		int todayYear = (int) instance.value(instances.attribute("TodayYear"));
		int todayMonth = (int) instance.value(instances.attribute("TodayMonth"));
		int todayDay = (int) instance.value(instances.attribute("TodayDay"));
		int targetYear = (int) instance.value(instances.attribute("TargetYear"));
		int targetMonth = (int) instance.value(instances.attribute("TargetMonth"));
		int targetDay = (int) instance.value(instances.attribute("TargetDay"));
		if (todayYear == targetYear && todayMonth == targetMonth && todayDay == targetDay)
		{
			double purchasePrice = instance.value(instances.attribute("PurchasePrice"));
			double purchaseQuantity = instance.value(instances.attribute("PurchaseQuantity"));
			double salePrice = instance.value(instances.attribute("SalePrice"));
			double saleQuantity = instance.value(instances.attribute("SaleQuantity"));
			double predict = salePrice * saleQuantity - purchasePrice * purchaseQuantity;
			
			suggestion = createSuggestion();
			Attribute classAttribute = instances.attribute(instances.numAttributes() - 1);
			String classAttributeName = classAttribute.name();
			suggestion.setFieldValue(classAttributeName, doubleToSuggestionField(classAttributeName, predict));
		}
		else
		{
			try
			{
				double predict = classifier.classifyInstance(instances.get(0));
				suggestion = createSuggestion();
				Attribute classAttribute = instances.attribute(instances.numAttributes() - 1);
				String classAttributeName = classAttribute.name();
				suggestion.setFieldValue(classAttributeName, doubleToSuggestionField(classAttributeName, predict));
			}
			catch (Exception e)
			{
				e.printStackTrace();
			}
		}
		return suggestion;
	}
}

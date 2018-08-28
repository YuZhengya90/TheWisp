package horus.datamining.model;

import horus.datamining.env.Environment;
import horus.datamining.model.feature.FeatureVector;


public class SaleQuantityPrediction extends AbstractLearningModel
{

	public SaleQuantityPrediction(Environment environment) throws Exception
	{
		super(environment);
	}


	@Override
	protected String getName()
	{
		return "SaleQuantityPrediction";
	}


	@Override
	protected void amendSuggestion(Suggestion suggestion, FeatureVector featureVector)
	{
		Number stockQuantity = (Number) featureVector.getValue("StockQuantity");
		Number saleQuantity = (Number) suggestion.getFieldValue("SalesQuantity");
		if (saleQuantity.intValue() > stockQuantity.intValue())
			suggestion.setFieldValue("SalesQuantity", stockQuantity.intValue());
	}
}

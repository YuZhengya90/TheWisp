package horus.datamining.model;

import horus.datamining.env.Environment;


public class PurchasePricePrediction extends AbstractLearningModel
{

	public PurchasePricePrediction(Environment environment) throws Exception
	{
		super(environment);
	}


	@Override
	protected String getName()
	{
		return "PurchasePricePrediction";
	}

}

package horus.datamining.model;

import horus.datamining.env.Environment;


public class SaleCommentsPrediction extends AbstractLearningModel
{

	public SaleCommentsPrediction(Environment environment) throws Exception
	{
		super(environment);
	}


	@Override
	protected String getName()
	{
		return "SaleCommentsPrediction";
	}

}

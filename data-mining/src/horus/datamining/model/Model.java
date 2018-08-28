package horus.datamining.model;

import horus.datamining.model.feature.FeatureVector;


public interface Model
{
	public FeatureVector createFeatureVector();


	public Suggestion solve(FeatureVector featureVector) throws Exception;
}

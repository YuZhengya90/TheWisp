package horus.datamining.model;

import java.util.*;
import horus.datamining.env.Environment;
import horus.datamining.model.feature.*;
import horus.datamining.model.feature.FeatureVectorImpl.Feature;
import weka.core.Attribute;
import weka.core.Instances;
import weka.core.converters.ConverterUtils.DataSource;


abstract class AbstractModel implements Model
{
	Environment environment;
	protected Instances featureSchema;


	public AbstractModel(Environment environment) throws Exception
	{
		this.environment = environment;
		featureSchema = DataSource.read(environment.getModelPath() + getFeatureSchemaFile());
	}


	abstract protected String getName();


	@Override
	public FeatureVector createFeatureVector()
	{
		List<Feature> features = getFeatures();
		FeatureVector featureVector = new FeatureVectorImpl(features);
		return featureVector;
	}


	protected List<Feature> getFeatures()
	{
		List<Feature> features = new LinkedList<Feature>();
		for (int i = 0; i < featureSchema.numAttributes(); ++i)
		{
			Attribute attribute = featureSchema.attribute(i);
			FeatureVectorImpl.Feature feature = new FeatureVectorImpl.Feature();

			feature.name = attribute.name();
			int attributeType = attribute.type();
			if (Attribute.NUMERIC == attributeType)
			{
				feature.type = FeatureType.NUMERIC;
				feature.value = 0.0;
			}
			else if (Attribute.NOMINAL == attributeType)
			{
				feature.type = FeatureType.NOMINAL;
				feature.validValues = new ArrayList<String>();
				for (int j = 0; j < attribute.numValues(); ++i)
				{
					feature.validValues.add(attribute.value(j));
				}
				feature.value = feature.validValues.get(0);
			}

			features.add(feature);
		}
		return features;
	}
	
	
	protected void adjustFeatureVector(FeatureVector featureVector)
	{}


	@Override
	public Suggestion solve(FeatureVector featureVector) throws Exception
	{
		if (null == featureVector)
			return null;

		adjustFeatureVector(featureVector);
		Instances instances = featureVector.toWekaInstances();
		Suggestion suggestion = solveInstances(instances);
		amendSuggestion(suggestion, featureVector);
		return suggestion;
	}


	abstract protected Suggestion solveInstances(Instances instances) throws Exception;


	protected Suggestion createSuggestion()
	{
		Attribute classAttribute = featureSchema.attribute(featureSchema.numAttributes() - 1);
		SuggestionImpl suggestion = new SuggestionImpl();
		suggestion.addField(classAttribute.name());
		return suggestion;
	}


	protected Object doubleToSuggestionField(String name, double value)
	{
		Attribute classAttribute = featureSchema.attribute(featureSchema.numAttributes() - 1);
		if (name.equals(classAttribute.name()))
			return new Double(value);
		else
			return null;
	}


	protected String getFeatureSchemaFile()
	{
		return getName() + ".arff";
	}


	protected void amendSuggestion(Suggestion suggestion, FeatureVector featureVector)
	{}
}

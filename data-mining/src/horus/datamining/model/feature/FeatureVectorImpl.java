package horus.datamining.model.feature;

import java.util.*;
import weka.core.*;


public class FeatureVectorImpl implements FeatureVector
{
	public static class Feature
	{
		public String name;
		public FeatureType type;
		public Object value;
		public List<String> validValues;


		public double valueToDouble()
		{
			double result = 0.0;
			if (FeatureType.NUMERIC == type)
			{
				Number number = (Number) value;
				result = number.doubleValue();
			}
			else if (FeatureType.NOMINAL == type)
			{
				for (String validValue : validValues)
				{
					if (value.equals(validValue))
						break;
					else
						result += 1.0;
				}
			}
			return result;
		}
	}

	private String name;
	private List<Feature> features;


	public FeatureVectorImpl(List<Feature> features)
	{
		this.features = features;
	}


	@Override
	public FeatureType getType(String name)
	{
		Feature feature = getFeature(name);
		if (null != feature)
			return feature.type;
		else
			return null;
	}


	@Override
	public Object getValue(String name)
	{
		Feature feature = getFeature(name);
		if (null != feature)
			return feature.value;
		else
			return null;
	}


	@Override
	public boolean setValue(String name, Object value)
	{
		Feature feature = getFeature(name);
		boolean validValueType = false;

		if (null != feature)
		{
			if (FeatureType.NOMINAL == feature.type && value instanceof String)
				validValueType = true;
			else if (FeatureType.NUMERIC == feature.type && (value instanceof Integer || value instanceof Double))
				validValueType = true;

			if (validValueType)
				feature.value = value;
		}

		return validValueType;
	}


	@Override
	public List<String> getValidValues(String name)
	{
		Feature feature = getFeature(name);
		if (null != feature && FeatureType.NOMINAL == feature.type)
			return new ArrayList<String>(feature.validValues);
		else
			return null;
	}


	@Override
	public Instances toWekaInstances()
	{
		ArrayList<Attribute> attributes = new ArrayList<Attribute>();
		for (Feature feature : features)
		{
			Attribute attribute = null;
			if (FeatureType.NUMERIC == feature.type)
				attribute = new Attribute(feature.name);
			else if (FeatureType.NOMINAL == feature.type)
			{
				ArrayList<String> labels = new ArrayList<String>();
				for (String validValue : feature.validValues)
				{
					labels.add(validValue);
				}
				attribute = new Attribute(feature.name, labels);
			}
			attributes.add(attribute);
		}
		Instances instances = new Instances(name, attributes, 1);
		instances.setClassIndex(instances.numAttributes() - 1);

		double[] values = new double[instances.numAttributes()];
		int i = 0;
		for (Feature feature : features)
		{
			values[i] = feature.valueToDouble();
			++i;
		}
		Instance instance = new DenseInstance(1.0, values);
		instances.add(instance);

		return instances;
	}


	protected Feature getFeature(String name)
	{
		// can optimize
		Feature result = null;
		for (Feature feature : features)
		{
			if (name.equals(feature.name))
			{
				result = feature;
				break;
			}
		}
		return result;
	}
}

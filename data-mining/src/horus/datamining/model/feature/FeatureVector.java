package horus.datamining.model.feature;

import java.util.List;
import weka.core.Instances;


public interface FeatureVector
{
	public FeatureType getType(String name);


	public Object getValue(String name);


	public boolean setValue(String name, Object value);


	public List<String> getValidValues(String name);


	Instances toWekaInstances();
}

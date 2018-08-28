package horus.datamining.model;

import java.util.Map;


public interface Suggestion
{
	public Object getFieldValue(String name);
	
	
	public void setFieldValue(String name, Object value);


	public Map<String, Object> getAllFields(); // name to value
}

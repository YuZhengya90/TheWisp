package horus.datamining.model;

import java.util.Map;
import java.util.TreeMap;


class SuggestionImpl implements Suggestion
{
	Map<String, Object> fields;


	SuggestionImpl()
	{
		fields = new TreeMap<String, Object>();
	}


	@Override
	public Object getFieldValue(String name)
	{
		return fields.get(name);
	}


	@Override
	public void setFieldValue(String name, Object value)
	{
		if (fields.containsKey(name))
			fields.put(name, value);
	}


	@Override
	public Map<String, Object> getAllFields()
	{
		return new TreeMap<String, Object>(fields);
	}


	void addField(String name)
	{
		fields.put(name, null);
	}
}

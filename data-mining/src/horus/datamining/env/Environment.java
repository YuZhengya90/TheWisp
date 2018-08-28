package horus.datamining.env;

public interface Environment
{
	public void setDatabaseUrl(String value);	// Format: jdbc:sap://10.58.114.58:30015


	String getDatabaseUrl();


	public void setDatabaseUser(String value);


	String getDatabaseUser();


	public void setDatabasePassword(String value);


	String getDatabasePassword();
	
	
	public void setModelPath(String value);
	
	
	String getModelPath();
}

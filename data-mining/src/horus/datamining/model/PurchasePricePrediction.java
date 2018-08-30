package horus.datamining.model;

import java.time.DayOfWeek;
import java.time.LocalDate;
import horus.datamining.env.Environment;
import horus.datamining.model.feature.FeatureVector;


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
	
	
	@Override
	protected void adjustFeatureVector(FeatureVector featureVector)
	{
		int year = ((Number) featureVector.getValue("Year")).intValue();
		int month = ((Number) featureVector.getValue("Month")).intValue();
		int day = ((Number) featureVector.getValue("Day")).intValue();
		LocalDate date = LocalDate.of(year, month, day);
		int weekDay = ((Number) featureVector.getValue("WeekDay")).intValue();
		int saturdayValue = DayOfWeek.SATURDAY.getValue() % DayOfWeek.SUNDAY.getValue();
		int sundayValue = DayOfWeek.SUNDAY.getValue() % DayOfWeek.SUNDAY.getValue();
		if (saturdayValue == weekDay)
		{
			date = date.plusDays(-1);
			featureVector.setValue("Year", date.getYear());
			featureVector.setValue("Month", date.getMonthValue());
			featureVector.setValue("Day", date.getDayOfMonth());
			int dayOfWeek = date.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
			featureVector.setValue("WeekDay", dayOfWeek);
		}
		else if (sundayValue == weekDay)
		{
			date = date.plusDays(-2);
			featureVector.setValue("Year", date.getYear());
			featureVector.setValue("Month", date.getMonthValue());
			featureVector.setValue("Day", date.getDayOfMonth());
			int dayOfWeek = date.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
			featureVector.setValue("WeekDay", dayOfWeek);
		}
	}

}

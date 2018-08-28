package horus.datamining.model;

import java.time.DayOfWeek;
import java.time.LocalDate;
import horus.datamining.env.Environment;
import horus.datamining.model.feature.FeatureVector;
import weka.core.Instances;


public class SalePricePrediction extends AbstractModel
{

	public SalePricePrediction(Environment environment) throws Exception
	{
		super(environment);
	}


	@Override
	protected String getName()
	{
		return "SalePricePrediction";
	}


	@Override
	public Suggestion solve(FeatureVector featureVector)
	{
		if (null == featureVector)
			return null;

		int year = (int) featureVector.getValue("Year");
		int month = (int) featureVector.getValue("Month");
		int day = (int) featureVector.getValue("Day");
		LocalDate date = LocalDate.of(year, month, day);
		Suggestion suggestion = createSuggestion();
		if (2017 == year)
		{
			if (date.isAfter(LocalDate.of(year, 3, 22)) && date.isBefore(LocalDate.of(year, 4, 17)))
				suggestion.setFieldValue("Price", 4.99);
			else
				suggestion.setFieldValue("Price", 5.99);
		}
		else
		{
			if (date.isAfter(getFirstThursdayAfterFirstSundayInApril(year).minusDays(1))
					&& date.isBefore(getFirstSundayInMay(year).plusDays(1)))
				suggestion.setFieldValue("Price", 4.99);
			else
				suggestion.setFieldValue("Price", 5.99);
		}
		return suggestion;
	}


	@Override
	protected Suggestion solveInstances(Instances instances)
	{
		return null;
	}


	private LocalDate getFirstThursdayAfterFirstSundayInApril(int year)
	{
		LocalDate date = LocalDate.of(year, 4, 1);
		while (DayOfWeek.SUNDAY != date.getDayOfWeek())
		{
			date = date.plusDays(1);
		}
		return date.plusDays(4);
	}


	private LocalDate getFirstSundayInMay(int year)
	{
		LocalDate date = LocalDate.of(year, 5, 1);
		while (DayOfWeek.SUNDAY != date.getDayOfWeek())
		{
			date = date.plusDays(1);
		}
		return date;
	}

}

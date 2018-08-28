drop procedure CALCULATE_INVENTORY;
create procedure CALCULATE_INVENTORY() as
dateVar timestamp;
endDate timestamp;
purchaseQuantity decimal(21,6);
salesQuantity decimal(21,6);
begin

drop table DATE2STOCK;
create column table DATE2STOCK("Date" timestamp, "StockQuantity" decimal(21,6));

dateVar = to_timestamp('2011-01-01', 'YYYY-MM-DD');
endDate = to_timestamp('2017-03-31', 'YYYY-MM-DD');
while dateVar <= endDate do
	select sum(O1."Quantity") into purchaseQuantity from OPOR O0 inner join POR1 O1 on O0."DocEntry" = O1."DocEntry" where O0."DocDate" <= dateVar;
	select sum(O1."Quantity") into salesQuantity from ORDR O0 inner join RDR1 O1 on O0."DocEntry" = O1."DocEntry" where O0."DocDate" <= dateVar;
	insert into DATE2STOCK values (dateVar, purchaseQuantity - salesQuantity);
	
	dateVar = ADD_DAYS(dateVar, 1);
end while;

end;



select
	year(R0."DocDate") "Year", month(R0."DocDate") "Month", dayofmonth(R0."DocDate") "Day", weekday(R0."DocDate") "WeekDay",
	TO_DECIMAL(R0."Comments", 21, 6) "Comments", R1."Price",
	ifnull(DS."StockQuantity", 0) + R1."Quantity" "StockQuantity",
	R1."Quantity" "SalesQuantity"
from HACKATHON6.ORDR R0 
inner join HACKATHON6.RDR1 R1 on R0."DocEntry" = R1."DocEntry" 
inner join HACKATHON6.DATE2STOCK DS on R0."DocDate" = DS."Date" 
order by R0."DocEntry";
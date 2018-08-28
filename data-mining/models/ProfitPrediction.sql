--DATE2STOCK is needed


--*******************************************
drop procedure GENERATE_PROFIT_DATA;
create procedure GENERATE_PROFIT_DATA() as
dateVar	timestamp;
endDate	timestamp;
cost	decimal(21,6);
income	decimal(21,6);
begin

drop table PROFIT_DATA;
create column table PROFIT_DATA(
	"Date"		timestamp,
	"Profit"	decimal(21,6)
);

dateVar = to_timestamp('2011-01-01', 'YYYY-MM-DD');
endDate = to_timestamp('2017-03-31', 'YYYY-MM-DD');
while dateVar <= endDate do
	select ifnull(O1."Quantity" * O1."Price", 0) into cost from DATE2STOCK DS left outer join OPOR O0 on O0."DocDate" = DS."Date" left outer join POR1 O1 on O0."DocEntry" = O1."DocEntry" where DS."Date" = dateVar;
	select ifnull(O1."Quantity" * O1."Price", 0) into income from DATE2STOCK DS left outer join ORDR O0 on O0."DocDate" = DS."Date" left outer join RDR1 O1 on O0."DocEntry" = O1."DocEntry" where DS."Date" = dateVar;
	insert into PROFIT_DATA values (dateVar, income - cost);

	dateVar = ADD_DAYS(dateVar, 1);
end while;

end;


--*******************************************
drop procedure GENERATE_PROFIT_TRAIN_DATA;
create procedure GENERATE_PROFIT_TRAIN_DATA() as
startDate		timestamp;
todayDate		timestamp;
targetDate		timestamp;
oneSeasonLater	timestamp;
endDate			timestamp;
stockQuantity		decimal(21,6);
purchasePrice		decimal(21,6);
purchaseQuantity	decimal(21,6);
comments			decimal(21,6);
customer			int;
salePrice			decimal(21,6);
saleQuantity		decimal(21,6);
profit				decimal(21,6);
begin

drop table PROFIT_TRAIN_DATA;
create column table PROFIT_TRAIN_DATA(
	"TodayDate"			timestamp,
	"StockQuantity"		decimal(21,6),
	"PurchasePrice"		decimal(21,6),
	"PurchaseQuantity"	decimal(21,6),
	"Comments"			decimal(21,6),
	"SalePrice"			decimal(21,6),
	"SaleQuantity"		decimal(21,6),
	"TargetDate"		timestamp,
	"Profit"			decimal(21,6)
);

startDate = to_timestamp('2011-01-01', 'YYYY-MM-DD');
todayDate = startDate;
endDate = to_timestamp('2017-03-31', 'YYYY-MM-DD');
while todayDate <= endDate do
	if todayDate = startDate then
		stockQuantity = 0;
	else
		select "StockQuantity" into stockQuantity from DATE2STOCK where "Date" = ADD_DAYS(todayDate, -1);
	end if;
	select ifnull(O1."Price", 0) into purchasePrice from DATE2STOCK DS left outer join OPOR O0 on O0."DocDate" = DS."Date" left outer join POR1 O1 on O0."DocEntry" = O1."DocEntry" where DS."Date" = todayDate;
	select ifnull(O1."Quantity", 0) into purchaseQuantity from DATE2STOCK DS left outer join OPOR O0 on O0."DocDate" = DS."Date" left outer join POR1 O1 on O0."DocEntry" = O1."DocEntry" where DS."Date" = todayDate;
	select ifnull(TO_DECIMAL("Comments", 21, 6), 0) into comments from DATE2STOCK DS left outer join ORDR O0 on O0."DocDate" = DS."Date" where DS."Date" = todayDate;
	select ifnull(O1."Price", 0) into salePrice from DATE2STOCK DS left outer join ORDR O0 on O0."DocDate" = DS."Date" left outer join RDR1 O1 on O0."DocEntry" = O1."DocEntry" where DS."Date" = todayDate;
	select ifnull(O1."Quantity", 0) into saleQuantity from DATE2STOCK DS left outer join ORDR O0 on O0."DocDate" = DS."Date" left outer join RDR1 O1 on O0."DocEntry" = O1."DocEntry" where DS."Date" = todayDate;
	
	targetDate = todayDate;
	oneSeasonLater = ADD_MONTHS(todayDate, 3);
	while targetDate <= oneSeasonLater and targetDate <= endDate do
		select sum("Profit") into profit from PROFIT_DATA where "Date" >= todayDate and "Date" <= targetDate;
		insert into PROFIT_TRAIN_DATA values (todayDate, stockQuantity, purchasePrice, purchaseQuantity, comments, salePrice, saleQuantity, targetDate, profit);
	
		targetDate = ADD_DAYS(targetDate, 1);
	end while;

	todayDate = ADD_DAYS(todayDate, 1);
end while;

end;


--*******************************************
select
	YEAR("TodayDate") "TodayYear",
	MONTH("TodayDate") "TodayMonth",
	DAYOFMONTH("TodayDate") "TodayDay",
	WEEKDAY("TodayDate") "TodayWeekDay",
	"StockQuantity",
	"PurchasePrice",
	"PurchaseQuantity",
	"Comments",
	"SalePrice",
	"SaleQuantity",
	YEAR("TargetDate") "TargetYear",
	MONTH("TargetDate") "TargetMonth",
	DAYOFMONTH("TargetDate") "TargetDay",
	WEEKDAY("TargetDate") "TargetWeekDay",
	DAYS_BETWEEN("TodayDate", "TargetDate") "Duration",
	"Profit"
from HACKATHON6.PROFIT_TRAIN_DATA
order by "TodayDate", "TargetDate";
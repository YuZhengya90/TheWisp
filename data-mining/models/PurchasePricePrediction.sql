select year(O0."DocDate") "Year", month(O0."DocDate") "Month", dayofmonth(O0."DocDate") "Day", weekday(O0."DocDate") "WeekDay", O1."Price" 
from HACKATHON6.OPOR O0 
inner join HACKATHON6.POR1 O1 on O0."DocEntry" = O1."DocEntry" 
where O0."DocDate" >= '20130101' 
order by O0."DocEntry";
select year(O0."DocDate") "Year", month(O0."DocDate") "Month", dayofmonth(O0."DocDate") "Day", O1."Price" 
from HACKATHON6.ORDR O0 
inner join HACKATHON6.RDR1 O1 on O0."DocEntry" = O1."DocEntry" 
where O0."DocDate" >= '20150101' 
order by O0."DocEntry";

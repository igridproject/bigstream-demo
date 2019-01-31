install.packages("devtools")
install.packages("jsonlite")
install.packages("zoo")

devtools::install_github("igridproject/Rbigstream")

library(Rbigstream)
library(xts)


host <- "http://demo.bs.igridproject.info/api"
storage_name <- "demo.weather.pathum"
token <- ""

bs.connect(host, token)
storage.list()
storage.stat(storage_name)

# Read last 5 days. 
df <- storage.read(storage_name,limit=50*5, last=50*5)
df <- df[,6:11]
df$temp <- as.numeric(df$temp)
df$ts<-as.POSIXct(df$ts, origin="1970-01-01")

par(mfrow=c(2,2))
tempts <- xts(df$temp, df$ts, format="%m%d%Y %H:%M")
plot(tempts, ylim=c(0,50) ,ylab="temperature (Celsius)", main="Temperature last 5 days")

abline(h=35,col="red")
abline(h=20,col="blue")

humts <- xts(df$hum, df$ts, format="%m%d%Y %H:%M")
plot(humts, ylim=c(0,120), ylab="Humidity %", main="Humidity last 5 days")
abline(h=70,col="red")

pressurets <- xts(df$pressure, df$ts, format="%m%d%Y %H:%M")
plot(pressurets, ylab="pressure ", main="Pressure last 5 days")

wind_speedts <- xts(df$wind_speed, df$ts, format="%m%d%Y %H:%M")
plot(wind_speedts,ylim=c(0,10) ,ylab="wind_speed ", main="wind_speed last 5 days")

import requests
import json

def api_response(city, if_pol): #check if pollution is called
    mkey = "b574c25bd6eb7f7802f69cd724e5308a"
    lat = ""
    lon = ""
    #those values are only returned if there is a problem e.g. no such city/region
    w_code = 9
    temperature = 99
    aircode = 9
    #1) Find city latitude (lat) and longitude (lon) from city.list.json
    with open ("./city.list.json", encoding="utf-8") as f:
        data = json.load(f) #data will be a dict
    for item in data:
        if item["name"].lower() == city.lower():
            #convert those floats to string to append them quicker
            lon = str(item["coord"]["lon"])
            lat = str(item["coord"]["lat"])
            break
    #2) Append lat and lon to the urls and call them
    try:
        if if_pol:
            airpol_url = "http://api.openweathermap.org/data/2.5/air_pollution?lat="+lat+"&lon="+lon+"&appid="+mkey
            a_response = requests.get(airpol_url)
            aircode = int (a_response.json()["list"][0]["main"]["aqi"]) #wrap just in case
        else:
            weather_url = "http://api.openweathermap.org/data/2.5/weather?lat="+lat+"&lon="+lon+"&appid="+mkey+"&units=metric"
            w_response = requests.get(weather_url)
            #parse responses
            weather = w_response.json()["weather"][0]["main"].lower()
            temperature = int (w_response.json()["main"]["temp"]) #wrap just in case
            icons = ['thunderstorm', 'rain', 'snow','clear', 'clouds', 'mist']
            if weather not in icons:
                weather = mist
            for i in range(0, len(icons)):
                if weather == icons[i]:
                    w_code = i
            if temperature < 0:
                temperature = 0       
    except KeyError:
        print("No such city/region.")
    #print(w_response.json())
    #print(a_response.json())    
    #if only air pollution is called, the other 2 are 0
    #vice versa, if temp/weather is called, aircode is 0
    return w_code, temperature, aircode
def weather(city):
    a,b,c = api_response(city, False)
    return int(a)
def temperature(city):
    a,b,c = api_response(city, False)
    return int(b)
def pollution(city):
    a,b,c = api_response(city, True)
    return int(c) #wrap a, b and c just in case the api decides to switch up

def testfff():
    icons = ['thunderstorm', 'rain', 'snow','clear', 'clouds', 'mist']
    city = "saigon"
    print(str(icons[weather(city)])) #int
    print(str(temperature(city))) #int
    print(str(pollution(city))) #int
    print(str(pollution('hanoi')))

#testfff()
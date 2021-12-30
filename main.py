import speech_recognition as sr
import subprocess
from apicall import *
import apicall
icons = ['thunderstorm', 'rain', 'snow','clear', 'clouds', 'mist']

def budget():
    global icons
    print("Speak with clear pauses in place of spaces!")
    listener = sr.Recognizer()
    while True:
        try:
            with sr.Microphone() as source:
                print("bot is listening...")
                listener.adjust_for_ambient_noise(source)   #wait for 1s before talking
                voice = listener.listen(source)
                cmd = listener.recognize_google(voice)
                print(">>VOICE: "+ cmd)
                cmd = cmd.lower()
                if 'off' in cmd:
                    print('bot is now off') 
                    subprocess.run(["python", "cereal.py", "c"]) #clear arduino screen
                    break
                if 'weather' in cmd:
                    index1 = cmd.index('weather')
                    city = cmd[(index1 + 8):]
                    dub = weather(city)
                    if dub != 9: #9 or 99 is code for error
                        result_w = 'w' + str(dub) #w0 - w5
                        subprocess.run(["python", "cereal.py", result_w])
                if 'temperature' in cmd:
                    index2 = cmd.index('temperature')
                    city = cmd[(index2 + 12):]
                    temp = temperature(city)
                    result_t = "t"
                    if temp != 99:
                        if temp < 10: #pad some zeros
                            result_t += "0" + str(temp)
                        else:
                            result_t += str(temp)
                        subprocess.run(["python", "cereal.py", result_t])
                if 'pollution' in cmd:
                    index3 = cmd.index('pollution')
                    city = cmd[(index3 +10):]
                    pol = pollution(city)
                    if pol != 9:
                        result_p = "p" + str(pol)
                        print(str(pol)) #is an int
                        subprocess.run(["python", "cereal.py", result_p])
        except sr.RequestError:   #API cannot be reached - connection problem?
            print("ERROR: Internet or API problem ")
        except sr.UnknownValueError:  #Unintelligible noises - coughing, sneezing, etc. or no noise at all
            print("ERROR: No voice, or voice cannot be transcribed")

budget()
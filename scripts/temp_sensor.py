#!/usr/bin/python
import sendgrid
import twilio
import time
import sys
import serial
from twilio.rest import TwilioRestClient

if(len(sys.argv) < 3):
	print("Usage: %s <phoneNumber> <emailAddress>" % sys.argv[0])

email = sys.argv[2]
phone = sys.argv[1]
SENDGRID_USERNAME = ""
SENDGRID_PASSWORD = ""

TWILIO_ACCT_SID = ""
TWILIO_AUTH_TOKEN = ""
TWILIO_NUMBER = ""


def sendEmail(email, temp):
	USERNAME = str(SENDGRID_USERNAME)
	PASSWORD = str(SENDGRID_PASSWORD)
	EMAIL_SENTTO = email
	EMAIL_SENTFROM = ""
	EMAIL_SUBJECT = "TEMPERATURE ALERT"
	MESSAGE = "<head><body><p>Hello, <br> The temperature of the roomis " + str(temp) + "<br>/p></body></head>"
	sg = sendgrid.SendGridClient(USERNAME, PASSWORD)
	message = sendgrid.Mail()
	message.add_to(EMAIL_SENTTO)
	message.set_from(EMAIL_SENTFROM)
	message.set_subject(EMAIL_SUBJECT)
	message.set_html(MESSAGE)
	status, message = sg.send(message)

	if(status == 200):
		print("Email has been SENT")

def sendText(phoneNumber, temp):
	ACCOUNT_SID = str(TWILIO_ACCT_SID)
	AUTH_TOKEN = str(TWILIO_AUTH_TOKEN)
	client = TwilioRestClient(ACCOUNT_SID, AUTH_TOKEN)
	MESSAGE = "Hello,\nThe Temperature of the room is " + str(temp)
	message = client.messages.create(body=MESSAGE, to=str(phoneNumber), from_=TWILIO_NUMBER)
	print(message.sid)

while True:
	ser = serial.Serial("/dev/cu.usbfmodem54321", 115200)
	serialLine = ser.readline()
	if(not serialLine.find("LOG")== -1):
		splittedLine = serialLine.split(" ")
		temp = float(splittedLine[len(splittedLine) -1])
		print(temp)
		sendEmail(email,temp)
		sendText(phone, temp)
		#time.sleep(50)

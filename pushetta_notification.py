# I only modified the lines 18, 19 and 20. Source: fpaez.com/enviar-notificaciones-push-con-raspberry-pi/
import urllib2
import json
import sys 

def sendNotification(token, channel, message):
        data = {
                "body" : message,
                "message_type" : "text/plain"
        }

        req = urllib2.Request('http://api.pushetta.com/api/pushes/{0}/'.format(channel))
        req.add_header('Content-Type', 'application/json')
        req.add_header('Authorization', 'Token {0}'.format(token))

        response = urllib2.urlopen(req, json.dumps(data))
# Source for the following: http://stackoverflow.com/questions/4429966/how-to-make-a-python-script-pipeable-in-bash
if __name__ == "__main__": 
    for line in sys.stdin:
        sendNotification("76543keykeykeykeykeykeykey28765465", "Stairs PIR", line)

import socket

HOST='localhost'
PORT= 1025

def send_request(sock, req):
    print(f'Sending request {req}')
    sock.sendall(req)

def check_response(sock, resp):
	recv_resp=sock.recv(256)
	#print(f'Got {recv_resp}')
	if recv_resp == resp:
	    print(f'OK: Received response {recv_resp}')
	    return True
	else:
	    print(f'ERROR: Expecting {resp}, got {recv_resp}')
	    return False

data=[(b'P\x07abc\x00\n,.', b'E\x07\xbf\xcf\xdd\xef\xd4\x81\x90'),
      (b'P\x0c\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00', b'E\x0c\xde\xad\xbe\xef\xde\xad\xbe\xef\xde\xad\xbe\xef'),
      (b'P\x08\xff\xff\xff\xff\xff\xff\xff\xff', b'E\x08!RA\x10!RA\x10')]

if __name__ == '__main__':
    for req, resp in data:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((HOST, PORT))
        send_request(sock, req)
        rv=check_response(sock, resp)
        sock.close()
        if not rv: break

all:
	g++ sws.cpp ServerLogic.cpp MessageFromClient.cpp MessageToClient.cpp LogResults.cpp Validator.cpp -o sws

clean:
	rm sws
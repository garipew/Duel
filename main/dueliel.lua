

if argc == 2 then
	print("APENAS CONECTAR")
	run_client(argv)
else
	print("HOST E CONECTAR")
	
	servfd = start_server()
	run_server(servfd, "0.0.0.0")
end

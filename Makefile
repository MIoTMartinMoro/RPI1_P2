all:
	gcc -o read read_bmp_ble.c read_hum_ble.c read_opt_ble.c read_tmp_ble.c read_sensors.c
	gcc -o cliente cliente_udp.c
	gcc -o servidor read_bmp_ble.c read_hum_ble.c read_opt_ble.c read_tmp_ble.c servidor_udp.c

clean:
	rm read servidor cliente
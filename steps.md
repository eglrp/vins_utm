~/Dropbox/VIO\ course/Assignments/week10/vins_sys_code/build$ make -j10
~/Dropbox/VIO\ course/Assignments/week10/vins_sys_code/bin$ ./run_euroc ~/DataSet/EuRoC/MH_05_difficult/mav0/ ../config/
~/Dropbox/VIO\ course/Assignments/week10/vins_sys_code/build$ make;../bin/testOpenMP

###EVO evaluation
~$ evo_ape euroc ~/DataSet/EuRoC/MH_05_difficult/mav0/state_groundtruth_estimate0/data.csv ~/Dropbox/VIO\ course/Assignments/week10/vins_sys_code/bin/test_logs/pose_output-MH05-LM-Single.txt -va --plot  --save_plot ./VINS-LM-Single.zip

### VINS-UTM
~/vins_utm/bin$ ./run_euroc ~/DataSet/EuRoC/MH_05_difficult/mav0/ ../config/

### HTTP test
~/vins_utm/bin$ ./testHTTP POST http://54.169.63.192/utmapi/api/pilotui/GetPilotFlights [\"thura1\"]


TODO


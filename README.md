# Computer-network-Ex_3
This exercise will allow you to create TCP server and client and to send files in diffrent Congestion Control algorithems the defaultive Cubic and Reno.

## Installation
to use this exercise you need to import the .c files, Makefile and .txt file to **your** linux and open the terminal at the same folder and wirte "make".
after you make the files you can write "make runs" to run the server and "make runc" to run the client.

## Usege
this exercise will give you the the tool to test the diffrentce between Cubic and Reno.
to resemble loss of packets we use tc tool in linux (to install - use the command "sudo apt install iproute2" ) by the command "sudo tc qdisc change dev lo root netem loss XX%".

## License
this exercise made by Ariel Yifee and Moti Dahari, to any question you can contact us by email - ariel.yifee@gmail.com or motidaharii@gmail.com

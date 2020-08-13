#This script is intended to reduce my battery overload

#These commands turn off my ehternet interface that is starting on every boot
sudo ip l s dev enp8s0 down

#This sets my keyboard when plugged on the specified port to battery safe mode
echo 'auto' | sudo tee '/sys/bus/usb/devices/1-3/power/control' > /dev/null

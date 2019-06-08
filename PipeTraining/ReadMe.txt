Implement a pipe that simulates the following command

tail -n +2 example | sed -r 's/([^\t]*)\t([^\t]*)/\2\t\1/' | sort | head -n 1

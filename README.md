# serial2ros

**serial2ros** is a ROS package that facilitates communication between a serial device and ROS nodes.

> [!WARNING]
> `serial` must be installed: `sudo apt install ros-<distro>-serial`.

## 🖥️ Usage
Data must be received as integers separated by semicolons (;). Each line of data is published as an `Int32MultiArray` message.

```bash
roslaunch serial2ros serial2ros.launch
```

> [!NOTE]
> Modify launch parameters in `serial2ros.launch` as needed: `port` and `baudrate`.

## 📝 License
Distributed under the Unlicense. See [`LICENSE`](https://github.com/raultapia/serial2ros/tree/main/LICENSE) for more information.

## 📬 Contact
[Raul Tapia](https://raultapia.com) - raultapia@us.es

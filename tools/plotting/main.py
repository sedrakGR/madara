from data_reader_interface import DataReaderFromFile
import data_reader_interface as dri
import visualizer as vis


filename = ("/media/sedrak/OS/SET/projects/"
                         "SAFE_reliability_rc_v3.4.0__pikachu-16-0004__2018-08-16-09-47-39.stk")
reader = DataReaderFromFile(filename)

prefix1 = '.gams.frames.p1_base_footprint'
suffix = '.origin'
keys = reader.get_keys()


x = []
y = []
z = []
u = []
v = []
w = []
for key in keys:
    if key.startswith(prefix1):
        last_value = ''
        count = 0
        while True:
            value, has_next = reader.get_current_value(key)

            x.append(value.retrieve_index(0).to_string())
            y.append(value.retrieve_index(1).to_string())
            z.append(value.retrieve_index(2).to_string())
            u.append(value.retrieve_index(3).to_string())
            v.append(value.retrieve_index(4).to_string())
            w.append(value.retrieve_index(5).to_string())

            if not(value.to_string() == last_value):
                last_value = value.to_string()
                print last_value
            if not(has_next):
                break
        # print(value.retrieve_index(0).to_string(), value.retrieve_index(1).to_string(), value.retrieve_index(2).to_string(),
            # value.retrieve_index(3).to_string(), value.retrieve_index(4).to_string(), value.retrieve_index(5).to_string())


vis.visualizer(x, y, z, u, v, w)
from pathlib import Path
from tkinter import Tk, Canvas, Entry, Text, Button, PhotoImage, Scale, font as tkFont
from tkinter import ttk
import serial
import serial.tools.list_ports
import threading
import serial.tools.list_ports
import datetime
import re
import time

OUTPUT_PATH = Path(__file__).parent
ASSETS_PATH = OUTPUT_PATH / Path(r"C:\Project HDL\New folder\build\assets\frame0")

def relative_to_assets(path: str) -> Path:
    return ASSETS_PATH / Path(path)

window = Tk()
window.title("Embedded Project")
window.iconbitmap('C:/Users/thien/OneDrive/Máy tính/BK/231/Project HDL/cse.ico')
window.geometry("1200x700")
window.configure(bg = "#FFFFFF")

canvas = Canvas(window, bg = "#FFFFFF", height = 700, width = 1200, bd = 0, highlightthickness = 0, relief = "ridge")
canvas.place(x = 0, y = 0)

image_image_1 = PhotoImage(file=relative_to_assets("image_1.png"))
image_1 = canvas.create_image(380.0, 630.0, image=image_image_1)

image_image_2 = PhotoImage(file=relative_to_assets("image_2.png"))
image_2 = canvas.create_image(600.0, 350.0, image=image_image_2)

image_image_3 = PhotoImage(file=relative_to_assets("image_3.png"))
image_3 = canvas.create_image(380.0, 360.0, image=image_image_3)

image_image_4 = PhotoImage(file=relative_to_assets("image_4.png"))
image_4 = canvas.create_image(840.0, 360.0, image=image_image_4)

button_image_1 = PhotoImage(file=relative_to_assets("button_1.png"))
button_1 = Button(image=button_image_1, borderwidth=0, highlightthickness=0,
                  command=lambda: print("button_1 clicked"), relief="flat")
button_1.place(x=810.0, y=330.0, width=60.0, height=60.0)

canvas.create_text(157.0, 35.0, anchor="nw", text="Port", fill="#000000", font=("Times", 36 * -1))
canvas.create_text(73.0, 125.0, anchor="nw", text="Baud Rate", fill="#000000", font=("Times", 36 * -1))
canvas.create_text(87.0, 215.0, anchor="nw", text="Stop Bits", fill="#000000", font=("Times", 36 * -1))
canvas.create_text(626.0, 35.0, anchor="nw", text="Data Bits", fill="#000000", font=("Times", 36 * -1))
canvas.create_text(606.0, 125.0, anchor="nw", text="Parity Bits", fill="#000000", font=("Times", 36 * -1))
canvas.create_text(890.0, 336.0, anchor="nw", text="Sync", fill="#000000", font=("Times", 36 * -1))
canvas.create_text(152.0, 340.0, anchor="nw", text="Date", fill="#000000", font=("Times", 36 * -1))
canvas.create_text(144.0, 420.0, anchor="nw", text="Time", fill="#000000", font=("Times", 36 * -1))
canvas.create_text(128.0, 500.0, anchor="nw", text="Alarm", fill="#000000", font=("Times", 36 * -1))
canvas.create_text(158.0, 610.0, anchor="nw", text="LED", fill="#000000", font=("Times", 36 * -1))

def refresh_com_ports():
    # Scan and update the COM ports in the combobox
    ports = [port.device for port in serial.tools.list_ports.comports()]
    current_selection = entry_2.get()

    if ports != entry_2['values']:
        entry_2['values'] = ports
        # If the currently selected port is still available, keep it selected
        if current_selection in ports:
            entry_2.set(current_selection)
        else:
            entry_2.set('')  # Clear the selection if the selected port is no longer available

    window.after(2000, refresh_com_ports)

custom_font = tkFont.Font(family="Times", size=24)
style = ttk.Style()
style.configure("TCombobox",
                fieldbackground="#E1F1FD",
                background="#E1F1FD",
                relief="flat",
                font=('Times', 20),
                padding="10 2 10 2",
                borderwidth=1,
                arrowsize=10,
                arrowcolor="#000716")

style.map('TCombobox',
          background=[('readonly', '#E1F1FD')],
          fieldbackground=[('readonly', '#E1F1FD')],
          foreground=[('readonly', '#000716')],
          bordercolor=[('readonly', '#B9D7EE')],
          arrowcolor=[('active', '#000716'), ('readonly', '#000716')])

# Adjust listbox appearance through option_add, Adjust height to show 5 items in the dropdown
window.option_add('*TCombobox*Listbox.font', ('Times', 15))
window.option_add('*TCombobox*Listbox.height', 8)

entry_image_2 = PhotoImage(file=relative_to_assets("entry_2.png"))
entry_bg_2 = canvas.create_image(380.0, 55.0, image=entry_image_2)
entry_2 = ttk.Combobox(window, style="TCombobox", state="readonly")
entry_2.place(x=290.0, y=25.0, width=180.0, height=60.0)
entry_2.configure(font=custom_font)

refresh_com_ports()

entry_image_3 = PhotoImage(file=relative_to_assets("entry_3.png"))
entry_bg_3 = canvas.create_image(380.0, 145.0, image=entry_image_3)
baud_rates = ["1200", "2400", "4800", "9600", "14400", "19200", "38400", "56000", "57600", "115200"]
entry_3 = ttk.Combobox(window, values=baud_rates, style="TCombobox", state="readonly", height=5)
entry_3.place(x=290.0, y=115.0, width=180.0, height=60.0)
entry_3.configure(font=custom_font)

entry_image_4 = PhotoImage(file=relative_to_assets("entry_4.png"))
entry_bg_4 = canvas.create_image(920.0, 55.0, image=entry_image_4)
data_bits = ["5", "6", "7", "8"]
entry_4 = ttk.Combobox(window, values=data_bits, style="TCombobox", state="readonly")
entry_4.place(x=830.0, y=25.0, width=180.0, height=60.0)
entry_4.configure(font=custom_font)

entry_image_5 = PhotoImage(file=relative_to_assets("entry_5.png"))
entry_bg_5 = canvas.create_image(920.0, 145.0, image=entry_image_5)
parity_bits = ["none", "even", "odd", "mark"]
entry_5 = ttk.Combobox(window, values=parity_bits, style="TCombobox", state="readonly")
entry_5.place(x=830.0, y=115.0, width=180.0, height=60.0)
entry_5.configure(font=custom_font)

entry_image_6 = PhotoImage(file=relative_to_assets("entry_6.png"))
entry_bg_6 = canvas.create_image(380.0, 235.0, image=entry_image_6)
stop_bits = ["0", "1"]
entry_6 = ttk.Combobox(window, values=stop_bits, style="TCombobox", state="readonly")
entry_6.place(x=290.0, y=205.0, width=180.0, height=60.0)
entry_6.configure(font=custom_font)

image_image_5 = PhotoImage(file=relative_to_assets("image_5.png"))
image_5 = canvas.create_image(920.0, 235.0, image=image_image_5)

image_image_6 = PhotoImage(file=relative_to_assets("image_6.png"))
image_6 = canvas.create_image(380.0, 440.0, image=image_image_6)

entry_image_1 = PhotoImage(file=relative_to_assets("entry_1.png"))
entry_bg_1 = canvas.create_image(660.0, 360.0, image=entry_image_1)
entry_1 = Entry(bd=0, bg="#ECEFFD", fg="#000716", highlightthickness=0, font=("Times", 26))
entry_1.place(x=540.0, y=328.0, width=240.0, height=60.0)

entry_image_7 = PhotoImage(file=relative_to_assets("entry_7.png"))
entry_bg_7 = canvas.create_image(660.0, 440.0, image=entry_image_7)
entry_7 = Entry(bd=0, bg="#ECEFFD", fg="#000716", highlightthickness=0, font=("Times", 26))
entry_7.place(x=540.0, y=408.0, width=240.0, height=60.0)

entry_image_8 = PhotoImage(file=relative_to_assets("entry_8.png"))
entry_bg_8 = canvas.create_image(660.0, 520.0, image=entry_image_8)
entry_8 = Entry(bd=0, bg="#ECEFFD", fg="#000716", highlightthickness=0, font=("Times", 26),)
entry_8.place(x=540.0, y=488.0, width=240.0, height=60.0)

image_image_7 = PhotoImage(file=relative_to_assets("image_7.png"))
image_7 = canvas.create_image(380.0, 520.0, image=image_image_7)

image_image_8 = PhotoImage(file=relative_to_assets("image_8.png"))
image_8 = canvas.create_image(925.0, 440.0, image=image_image_8)

button_image_4 = PhotoImage(file=relative_to_assets("button_4.png"))
button_4 = Button(image=button_image_4, borderwidth=0, highlightthickness=0,
                  command=lambda: print("button_4 clicked"), relief="flat")
button_4.place(x=835.0, y=415.0, width=180.0, height=50.0)

image_image_9 = PhotoImage(file=relative_to_assets("image_9.png"))
image_9 = canvas.create_image(925.0, 520.0, image=image_image_9)

button_image_5 = PhotoImage(file=relative_to_assets("button_5.png"))
button_5 = Button( image=button_image_5, borderwidth=0, highlightthickness=0,
                   command=lambda: print("button_5 clicked"), relief="flat")
button_5.place(x=825.0, y=495.0, width=200.0, height=50.0)

image_image_10 = PhotoImage(file=relative_to_assets("image_10.png"))
image_10 = canvas.create_image(380.0, 630.0, image=image_image_10)

slider = Scale(window, from_=1, to=5, orient='horizontal',
               showvalue=False,
               bg='#B8BFFA',
               fg='#ffffff',
               highlightbackground='#E1F1FD',
               troughcolor='#E1F1FD',
               sliderrelief='flat',
               sliderlength=50,
               length=280, width=30)
slider.set(5)
slider.place(x=517, y=590)

value_label = ttk.Label(
    window,
    text="Current Level: 5",
    font=("Times", 16),
    foreground='#000000',
    background='#E1F1FD')
value_label.place(x=517, y=639)


Brightness_From_UART = 0
Brightness_Current_Value = 0
def Update_Brightness_Display():
    global Brightness_Current_Value, Response, Brightness_From_UART

    if Brightness_From_UART == 0:
        Brightness_Current_Value = int(slider.get())
        value_label.config(text=f"Current Level: {Brightness_Current_Value}")
        Combined_String = f"Set Level:{Brightness_Current_Value}!"

        send_data(Combined_String)
    else:
        Brightness_Current_Value = int(''.join([i for i in Response if i.isdigit()]))
        value_label.config(text=f"Current Level: {Brightness_Current_Value}")
        slider.set(Brightness_Current_Value)
        Brightness_From_UART = 0

def Brightness_Display(event):
    Update_Brightness_Display()

slider.bind("<ButtonRelease-1>", Brightness_Display)


Flag_Connect = 0
Check_Valid1 = 0
Check_Valid2 = 0
Check_Valid3 = 0
Combined_String = ""
Response = ""
Date = " 29.08.2024"
Time = "12-00-00"
Alarm = ""
Alarm_On = 0

def is_valid_date(date_str):
    if not re.match(r"^\d{2}\.\d{2}\.\d{4}$", date_str):
        return False

    try:
        day, month, year = map(int, date_str.split('.'))
        datetime.datetime(year, month, day)
    except ValueError:
        return False

    return True

def is_valid_time(time_str):
    if not re.match(r"^\d{2}-\d{2}-\d{2}$", time_str):
        return False

    try:
        hour, minute, second = map(int, time_str.split('-'))
        if not (0 <= hour < 24 and 0 <= minute < 60 and 0 <= second < 60):
            return False
    except ValueError:
        return False

    return True

def Connect_UART():
    global Flag_Connect, Combined_String

    if Flag_Connect == 0:
        port = entry_2.get()
        baud_rate = entry_3.get()
        data_bits = entry_4.get()
        parity_bits = entry_5.get()
        start_bits = entry_6.get()

        Combined_String = f"Connect:COM/{baud_rate}/{data_bits}/{parity_bits}/{start_bits}!"

        send_data(Combined_String)

        Flag_Connect = 1
        button_3.place_forget()
        button_6.place(x=830.0, y=210.0, width=180.0, height=50.0)
        button_6.focus_set()
    else:
        Flag_Connect = 0
        button_6.place_forget()
        button_3.place(x=830.0, y=210.0, width=180.0, height=50.0)
        button_3.focus_set()
        enable_entry(entry_2)
        enable_entry(entry_3)
        enable_entry(entry_4)
        enable_entry(entry_5)
        enable_entry(entry_6)

button_image_3 = PhotoImage(file=relative_to_assets("button_3.png"))
button_3 = Button(image=button_image_3, borderwidth=0, highlightthickness=0,
                  command=Connect_UART, relief="flat")
button_3.place(x=830.0, y=210.0, width=180.0, height=50.0)
button_image_6 = PhotoImage(file=relative_to_assets("button_6.png"))
button_6 = Button(image=button_image_6, borderwidth=0, highlightthickness=0,
                  command=Connect_UART, relief="flat")

def Set_Time_UART():
    global Combined_String, Date, Time, Check_Valid1, Check_Valid2, entry_image_1, entry_image_7

    Check_Valid1 = Check_Valid2 = 0

    set_date = entry_1.get()
    set_time = entry_7.get()

    if not is_valid_date(set_date) and Flag_Syn == 0:
        print("Error: Invalid date format. Use DD.MM.YY.")
        entry_1.delete(0, 'end')
        entry_image_error1 = PhotoImage(file=relative_to_assets("entry_error.png"))
        canvas.create_image(660.0, 360.0, image=entry_image_error1, tags="entry_error1_bg")
        entry_image_error1.image = entry_image_error1
        entry_1.place(x=540.0, y=328.0, width=240.0, height=60.0)
        Check_Valid1 = 1

    if not is_valid_time(set_time) and Flag_Syn == 0:
        print("Error: Invalid time format. Use HH-MM-SS.")
        entry_7.delete(0, 'end')
        entry_image_error2 = PhotoImage(file=relative_to_assets("entry_error.png"))
        canvas.create_image(660.0, 440.0, image=entry_image_error2, tags="entry_error2_bg")
        entry_image_error2.image = entry_image_error2
        entry_7.place(x=540.0, y=408.0, width=240.0, height=60.0)
        Check_Valid2 = 1

    if Check_Valid1 == 1 and Check_Valid2 == 1:
        return

    if set_date == "":
        set_date = "null"
    if set_time == "":
        set_time = "null"

    if set_date != "null" or set_time != "null":
        if Flag_Connect == 1:
            if set_date != "null":
                Date = set_date
            if set_time != "null":
                Time = set_time
            Combined_String = f"Set Time:{set_date}/{set_time}!"

            send_data(Combined_String)

            entry_image_1 = PhotoImage(file=relative_to_assets("entry_1.png"))
            canvas.create_image(660.0, 360.0, image=entry_image_1, tags="entry_bg_1")
            entry_1.place(x=540.0, y=328.0, width=240.0, height=60.0)
            entry_image_7 = PhotoImage(file=relative_to_assets("entry_7.png"))
            canvas.create_image(660.0, 440.0, image=entry_image_7, tags="entry_bg_7")
            entry_7.place(x=540.0, y=408.0, width=240.0, height=60.0)

    if Flag_Syn == 1:
        current_date = datetime.date.today()
        Date = current_date.strftime("%d.%m.%Y")
        current_time = datetime.datetime.now().time()
        Time = current_time.strftime("%H-%M-%S")
        Combined_String = f"Set Time:{Date}/{Time}!"

        send_data(Combined_String)

        entry_image_1 = PhotoImage(file=relative_to_assets("entry_1.png"))
        canvas.create_image(660.0, 360.0, image=entry_image_1, tags="entry_bg_1")
        entry_1.place(x=540.0, y=328.0, width=240.0, height=60.0)
        entry_image_7 = PhotoImage(file=relative_to_assets("entry_7.png"))
        canvas.create_image(660.0, 440.0, image=entry_image_7, tags="entry_bg_7")
        entry_7.place(x=540.0, y=408.0, width=240.0, height=60.0)

    button_4.focus_set()

button_4.config(command=Set_Time_UART)

def Set_Alarm_UART():
    global Combined_String, Alarm, Check_Valid3, entry_image_8, Alarm_On, button_image_5

    if Alarm_On == 1:
        enable_entry(entry_1)
        enable_entry(entry_7)
        button_1.config(state="normal")
        button_4.config(state="normal")
        if entry_8.get() == "":
            Combined_String = f"OFF Alarm!"
            send_data(Combined_String)
            return

    Check_Valid3 = 0

    set_alarm = entry_8.get()

    if not is_valid_time(set_alarm) and Flag_Syn == 0:
        print("Error: Invalid time format. Use HH-MM-SS.")
        entry_8.delete(0, 'end')
        entry_image_error3 = PhotoImage(file=relative_to_assets("entry_error.png"))
        canvas.create_image(660.0, 520.0, image=entry_image_error3, tags="entry_error3_bg")
        entry_image_error3.image = entry_image_error3
        entry_8.place(x=540.0, y=488.0, width=240.0, height=60.0)
        Check_Valid3 = 1

    if Check_Valid3 == 1:
        return

    if set_alarm != "" and Flag_Connect == 1:
        Alarm = set_alarm
        Combined_String = f"Set Alarm:{set_alarm}!"

        send_data(Combined_String)

        entry_image_8 = PhotoImage(file=relative_to_assets("entry_8.png"))
        canvas.create_image(660.0, 520.0, image=entry_image_8, tags="entry_bg_8")
        entry_8.place(x=540.0, y=488.0, width=240.0, height=60.0)

    entry_8.delete(0, 'end')
    button_5.focus_set()

button_5.config(command=Set_Alarm_UART)

def disable_entry(entry):
    entry.delete(0, 'end')
    entry.bind("<Key>", lambda e: "break")


def enable_entry(entry):
    entry.unbind("<Key>")

Flag_Syn = 0

def Synchronize():
    global Flag_Syn

    if Flag_Syn == 1:
        Flag_Syn = 0
        button_2.place_forget()
        button_1.place(x=810.0, y=330.0, width=60.0, height=60.0)
        enable_entry(entry_1)
        enable_entry(entry_7)
        enable_entry(entry_8)
    else:
        Flag_Syn = 1
        button_1.place_forget()
        button_2.place(x=810.0, y=330.0, width=60.0, height=60.0)
        disable_entry(entry_1)
        disable_entry(entry_7)
        disable_entry(entry_8)

button_image_1 = PhotoImage(file=relative_to_assets("button_1.png"))
button_1 = Button(image=button_image_1, borderwidth=0, highlightthickness=0,
                      command=Synchronize, relief="flat")
button_1.place(x=810.0, y=330.0, width=60.0, height=60.0)
button_image_2 = PhotoImage(file=relative_to_assets("button_2.png"))
button_2 = Button(image=button_image_2, borderwidth=0, highlightthickness=0,
                  command=Synchronize, relief="flat")

def Alarm_Display():
    global button_image_5, Alarm_On, image_image_1

    if Alarm_On == 1:
        disable_entry(entry_1)
        disable_entry(entry_7)
        button_1.config(state="disabled")
        button_4.config(state="disabled")

    blinking_thread = threading.Thread(target=Blink_Alarm, daemon=True)
    blinking_thread.start()
    
def Blink_Alarm():
    global image_image_1, Alarm_On, image1

    toggle = True
    while Alarm_On == 1:
        if toggle:
            image_image_1 = PhotoImage(file=relative_to_assets("image_1.png"))
        else:
            image_image_1 = PhotoImage(file=relative_to_assets("image_10.png"))
        toggle = not toggle
        time.sleep(0.5)

def Display_Date_Time_Alarm():
    global Date, Time, Alarm

    canvas.delete("date_text")
    canvas.delete("time_text")
    canvas.delete("alarm_text")

    if Date != "" and Flag_Connect == 1:
        canvas.create_text(380, 360, text=Date, fill="#000000", font=("Times", 26 * -1), tags="date_text")

    if Time != "" and Flag_Connect == 1:
        canvas.create_text(380, 440, text=Time, fill="#000000", font=("Times", 26 * -1), tags="time_text")

    if Alarm != "" and Flag_Connect == 1:
        canvas.create_text(380, 520, text=Alarm, fill="#000000", font=("Times", 26 * -1), tags="alarm_text")

ser = serial.Serial(
    port='COM1',
    baudrate=38400,
    parity=serial.PARITY_EVEN,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1)

def send_data(data):
    ser.write(data.encode())
    print(f"Sent: {data}")

def receive_data():
    global Response

    Response = ser.readline().decode()
    if "Current Time" not in Response:
        print(f"Received: {Response}")

def UART_Communication():
    global Combined_String, Response, Flag_Connect, Brightness_From_UART, Time, Alarm_On

    while True:
        Display_Date_Time_Alarm()
        receive_data()
        if "Wrong" in Response:
            Flag_Connect = 1
            Connect_UART()
        if "Alarm ON" in Response:
            Alarm_On = 1
            Alarm_Display()
        if "Finish" in Response:
            Alarm_On = 0
            enable_entry(entry_1)
            enable_entry(entry_7)
            button_1.config(state="normal")
            button_4.config(state="normal")
        if "Current Time:" in Response:
            Time = Response.split("Current Time: ")[1].strip()
            canvas.delete("time_text")
            canvas.create_text(380, 440, text=Time, fill="#000000", font=("Times", 26 * -1), tags="time_text")
        if "Adjust Brightness:" in Response:
            Brightness_From_UART = 1
            Update_Brightness_Display()

def start_uart_thread():
    uart_thread = threading.Thread(target=UART_Communication, daemon=True)
    uart_thread.start()

start_uart_thread()

window.resizable(False, False)
window.mainloop()

# # Create a variable to store the after_id to reset the timer
# delayed_calls = {}
#
# def delayed_print(entry_name, entry):
#     print(f"{entry_name}: {entry.get()}")
#
# def on_key_release(event, entry_name, entry):
#     # If there's a scheduled call for this entry, cancel it
#     if entry_name in delayed_calls:
#         window.after_cancel(delayed_calls[entry_name])
#
#     # Schedule a new delayed call
#     delayed_calls[entry_name] = window.after(1000, delayed_print, entry_name, entry)
#
# # Bind each entry widget to the 'on_key_release' function
# entry_1.bind("<KeyRelease>", lambda event: on_key_release(event, "Entry 1", entry_1))
# entry_2.bind("<KeyRelease>", lambda event: on_key_release(event, "Entry 2", entry_2))
# entry_3.bind("<KeyRelease>", lambda event: on_key_release(event, "Entry 3", entry_3))
# entry_4.bind("<KeyRelease>", lambda event: on_key_release(event, "Entry 4", entry_4))
# entry_5.bind("<KeyRelease>", lambda event: on_key_release(event, "Entry 5", entry_5))
# entry_6.bind("<KeyRelease>", lambda event: on_key_release(event, "Entry 6", entry_6))
# entry_7.bind("<KeyRelease>", lambda event: on_key_release(event, "Entry 7", entry_7))
# entry_8.bind("<KeyRelease>", lambda event: on_key_release(event, "Entry 8", entry_8))

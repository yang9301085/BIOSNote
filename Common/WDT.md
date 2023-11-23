## **1. What is a watchdog timer (WDT: Watchdog timer)**

A watchdog timer (WDT) is a timer that monitors microcontroller (MCU) programs to see if they are out of control or have stopped operating. It acts as a “watchdog” watching over MCU operation.

A microcontroller (MCU) is a compact processor for controlling electronic devices. Integrated into a wide variety of electronic devices, MCUs come pre-loaded with program software whose commands are used to control electronic devices.  
This makes safeguarding normal MCU operation essential. Should the MCU program, for some reason, run out of control or stop running altogether, the electronic device may behave erratically, which in the worst case could cause damage or an accident.

To proactively prevent such incidents, **it falls to the role of the watchdog timer to constantly watch over the MCU to ensure it is operating normally.**

The watchdog timer function can be inside the MCU, but here we are introducing “external” watchdog timers, the safer kind.

## **2. WDT operation : Detection of MCU faults**

The watchdog timer communicates with the MCU at a set interval. If the MCU does not output a signal, outputs too many signals or outputs signals that differ from a predetermined pattern, the timer determines that the MCU is malfunctioning and sends a reset signal to the MCU.

[![Circuits peripheral to the MCU and WDT (in an automotive environment).The WDT takes the role of “watchdog” and watches over MCU operation at all times.](https://www.ablic.com/en/semicon/wp-content/uploads/2020/01/wdt_mcu_en.png)](https://www.ablic.com/en/semicon/wp-content/uploads/2020/01/wdt_mcu_en.png)

The WDT uses a number of methods (modes) to detect MCU faults and the type of faults it detects varies with the mode.The following is a description of WDT operation and features by mode.

### **Time-out mode**

[![WDT operation (Time-out mode)](https://www.ablic.com/en/semicon/wp-content/uploads/2020/01/wdt_operation_timeout_en.png)](https://www.ablic.com/en/semicon/wp-content/uploads/2020/01/wdt_operation_timeout_en.png)

In this mode, the watchdog timer determines the MCU is malfunctioning and outputs a reset signal **if it does not receive a signal from the MCU within the set interval.**

[![タイムアウトモードが異常を検出できない例](https://www.ablic.com/en/semicon/wp-content/uploads/2020/01/wdt_operation_timeout_2_en.png)](https://www.ablic.com/en/semicon/wp-content/uploads/2020/01/wdt_operation_timeout_2_en.png)

The time-out mode is a major WDT monitoring mode or method, but it sometimes fails to detect MCU faults.  
In Time-out mode, the WDT will not detect an MCU fault if the MCU inputs multiple signals (= double pulse) in the set period.

### **Window mode**

[![WDT operation (Window mode)](https://www.ablic.com/en/semicon/wp-content/uploads/2020/01/wdt_operation_window_en.png)](https://www.ablic.com/en/semicon/wp-content/uploads/2020/01/wdt_operation_window_en.png)

The window mode enables more accurate detection of faults than the Time-out mode.  
In window mode, the watchdog timer determines that the MCU is malfunctioning and outputs a reset signal **if it does not receive a signal, or receives multiple signals (= double pulse) from the MCU within the set interval.**

A window mode watchdog timer may be more suitable for applications such as automotive devices that require greater safety.

  
  

### **Q&A (Question & Answer) mode**

[![WDT operation (Q&A mode)](https://www.ablic.com/en/semicon/wp-content/uploads/2020/01/wdt_operation_q-and-a_en.png)](https://www.ablic.com/en/semicon/wp-content/uploads/2020/01/wdt_operation_q-and-a_en.png)

The Q&A mode enables more accurate detection of faults than the previous two modes.

In Q&A mode, the MCU sends predetermined data to the WDT. **The WDT determines whether or not the MCU is operating normally depending on whether or not the signal sent by the MCU matches predetermined data.**

Devices that require a high degree of safety, may require a Q&A mode WDT. However, unlike the window and the timeout modes, this mode relies on data communication between the MCU and WDT, which makes operation more complex.

## **3. Selecting a WDT**

In the following, we will bring up points that should be considered before deciding which WDT to choose.

### **Is an “external” WDT necessary?**

The WDT function for detecting MCU malfunction is also included in the MCU itself.  
Is there then any need to provide an external WDT in addition to the WDT functions already built into an MCU?

[![冗長性として「外付けのWDT」が必要になるケースも](https://www.ablic.com/en/semicon/wp-content/uploads/2020/01/wdt_inhoused-in-mcu_en.png)](https://www.ablic.com/en/semicon/wp-content/uploads/2020/01/wdt_inhoused-in-mcu_en.png)

The reason is that **an external WDT adds an extra layer of safety to critical systems.**  
As said earlier, a WDT monitors an MCU to detect faults. When an MCU is monitored only by a WDT built into the MCU, there is no guarantee that a failing MCU will be able to monitor itself and also detect faults.

An independently operating WDT is the only way to resolve such concerns.  
Redundancy is considered vital in maintaining system safety. And an external WDT can provide the required redundancy.

  

### **What applications require WDTs?**

MCUs are used in all sorts of electronic devices, but whether a WDT is required or not depends on the “level of safety demanded by or deemed necessary for a specific application.”

Automotive devices are devices where MCU failure or malfunction could lead to life-threatening accidents. In water heaters and kitchen stoves, MCU failure or malfunction poses a fire risk.

**In systems that impact human life or in applications where malfunction of electronic control can cause serious accidents, an external WDT is required to ensure sufficient redundancy.**

International standards such as **ISO 26262 emphasize that the concept of “functional safety” is essential in “ensuring system safety if safety-related functions and parts fail.”**Functional safety requires the installation of mechanisms (safety devices) for detecting issues such as part malfunction to lower risk to the level of acceptable risk.  
Use of a WDT makes it possible to detect MCU program and other faults and design safety into the entire system.

  

### **Which mode of WDT is best?**

Again, like applications that require WDTs, some situations may require a WDT to provide the level of safety that is required or deemed necessary to implement.

The safety demanded of electronic devices is becoming ever more rigorous, especially in the automotive field. For example, new models of devices for which timeout mode WDTs used to be standard are now being replaced with window-mode WDTs.

In configuring safe systems, **external WDTs are needed for redundancy and if they are window-mode WDTs, they will provide highly accurate monitoring and detection.** These are some of the points you should consider when selecting your WDT.
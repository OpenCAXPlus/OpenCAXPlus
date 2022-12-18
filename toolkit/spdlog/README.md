It is not possible to use va_list in spdlog. 
We use vsnprintf as a workaround, which format the string in printf, then use spdlog to print the formatted string.

In this way, the spdlog function conform to our interface, but you no longer has the full benefit of spdlog and fmt.

To fully utilize spdlog, you can use it directly in your app without using the OPC interface.
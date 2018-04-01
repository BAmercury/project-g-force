using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ArduinoDriver;
using ArduinoDriver.SerialProtocol;
using ArduinoUploader.Hardware;
using ArduinoUploader;
using RJCP.IO.Ports;
using System.IO;
using System.Text.RegularExpressions;

namespace RPM_logger
{
    class Program
    {
        static void Main(string[] args)
        {

            List<List<double>> data = new List<List<double>>();

            Console.WriteLine("Starting Up Program");

            string serialPortName = "COM9";
            var allPortNames = SerialPortStream.GetPortNames();
            var distinctPorts = allPortNames.Distinct().ToList();

            // If we don't specify a COM port, automagically select one if there is only a single match.
            if (distinctPorts.SingleOrDefault() != null)
            {
                serialPortName = distinctPorts.Single();
            }



                SerialPortStream port = new SerialPortStream(serialPortName, 115200);
                port.Open();
                bool quick = true;
                while (quick)
                {
                    if (port.BytesToRead > 0 )
                    {
                        string s = port.ReadLine();
                        s = Regex.Replace(s, @"\r", string.Empty);
                        Console.WriteLine("Connecting to Arduino");
                        if (s == "ready")
                        {
                            quick = false;
                            Console.WriteLine("Device Ready");
                            break;
                        }
                    }
                }

                port.Write("<1>");
                bool quick_three = true;
                while (quick_three)
                {
                    if (port.BytesToRead > 0)
                    {
                        string s = port.ReadLine();
                        s = Regex.Replace(s, @"\r", string.Empty);

                        if (s == "give")
                        {
                        //double position = 30.00;
                            Console.WriteLine("Input desired speed");
                            string user_input = Console.ReadLine();
                            //double desired_pos = Convert.ToDouble(user_input);
                            port.WriteLine(user_input);
                            quick_three = false;
                        //Console.WriteLine("lol");
                        //port.Flush();
                            break;
                        }
                    }
                }
                bool stop = true;
            Console.WriteLine("Test Started");
                while (stop)
                {
                    if (port.BytesToRead > 0)
                    {
                        string s = port.ReadLine();
                        s = Regex.Replace(s, @"\r", string.Empty);
                    if (s == "end")
                        {
                            stop = false;
                            break;
                        }
                        string[] message = s.Split(',');
                        List<double> temp = new List<double>();
                        //temp.Add(Convert.ToDouble(s));
                        foreach (string element in message)
                        {

                            //Console.WriteLine(element);

                            double value = Convert.ToDouble(element);
                            temp.Add(value);
                            Console.WriteLine(element);

                        }
                        data.Add(temp);

                    }




                }

            Console.WriteLine("here");
            using (TextWriter tw = new StreamWriter("SavedList.csv"))
            {
                foreach (List<double> member in data)
                {
                    //tw.Write(member);

                    foreach (double guy in member)
                    {
                        tw.Write(guy);
                        tw.Write(",");
                    }
                    tw.WriteLine();
                }
            }
            Console.WriteLine("Wrote to file");


        }

        }
    }


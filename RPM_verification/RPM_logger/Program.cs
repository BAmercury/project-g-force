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


        List<double> poll_for_data(SerialPortStream port, string user_input)
        {
            // Send Command to Start, send it desired speed to run at
            port.Write("<1>");
            bool quick_three = true;
            while (quick_three)
            {
                if (port.BytesToRead > 0)
                {
                    string s = port.ReadLine();
                    s = Regex.Replace(s, @"\r", string.Empty);

                    // Either prompt user for input, or take in function line arugment for a desired RPM to run at
                    if (s == "give")
                    {
                        //double position = 30.00;
                        if (user_input.Length < 0)
                        {
                            Console.WriteLine("Input desired delay span");
                            string manual_user_input = Console.ReadLine();
                            //double desired_pos = Convert.ToDouble(user_input);
                            port.WriteLine(manual_user_input);

                        }
                        else
                        {
                            port.WriteLine(user_input);
                        }

                        quick_three = false;
                        break;
                    }
                }
            }
            bool stop = true;
            Console.WriteLine("Test Started");

            List<double> temp = new List<double>();
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

                    //temp.Add(Convert.ToDouble(s));
                    foreach (string element in message)
                    {

                        //Console.WriteLine(element);

                        double value = Convert.ToDouble(element);
                        temp.Add(value);
                        Console.WriteLine(element);

                    }

      
                    //data.Add(temp);

                }




            }

            return temp;

        }

        }
    }


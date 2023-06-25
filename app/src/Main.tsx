import React from "react";
import { NativeEventEmitter, NativeModules, Text, View } from "react-native";

import {
  NORDIC_UART_SERVICE_UUID,
  RX_CHAR_UUID,
  SCAN_DURATION_SECONDS,
} from "../constants";

const BleManagerModule = NativeModules.BleManager;
const bleManagerEmitter = new NativeEventEmitter(BleManagerModule);

export default function MainScreen(): JSX.Element {
  // const [peripherals, setPeripherals] = useState<Peripheral[]>([]);

  // useEffect(() => {
  //   BleManager.start().then(scan);
  // }, []);

  // const handleDiscoverPeripheral = (peripheral: Peripheral) => {
  //   if (!peripheral.name) {
  //     peripheral.name = "NO NAME";
  //   }
  //   setPeripherals([peripheral, ...peripherals]);
  // };

  // const listeners = [
  //   bleManagerEmitter.addListener(
  //     "BleManagerDiscoverPeripheral",
  //     handleDiscoverPeripheral
  //   ),
  // ];

  // function removeListeners() {
  //   listeners.forEach((l) => {
  //     l.remove();
  //   });
  // }

  // function scan() {
  //   BleManager.scan([NORDIC_UART_SERVICE_UUID], SCAN_DURATION_SECONDS, false);
  // }

  // function stringToByteArray(str: string): number[] {
  //   let byteArray = new Uint8Array(str.length);
  //   for (let i = 0; i < str.length; i++) {
  //     byteArray[i] = str.charCodeAt(i) & 0xff;
  //   }
  //   return Array.from(byteArray);
  // }

  // async function connectAndTransmit() {
  //   console.info(`connecting to ${peripherals[0].id}...`);
  //   await BleManager.connect(peripherals[0].id);
  //   const peripheralData = await BleManager.retrieveServices(peripherals[0].id);
  //   await BleManager.write(
  //     peripherals[0].id,
  //     NORDIC_UART_SERVICE_UUID,
  //     RX_CHAR_UUID,
  //     // Transmit value for "Black Beauty 8B"
  //     stringToByteArray("l#S175,P85,P152,E126#")
  //   );
  // }

  return (
    <View style={{ flex: 1, alignItems: "center", justifyContent: "center" }}>
      <Text>Main</Text>
    </View>
  );
}

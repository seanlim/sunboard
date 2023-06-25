import React from "react";
import BleManager, { Peripheral } from "react-native-ble-manager";
import {
  Button,
  FlatList,
  ListRenderItemInfo,
  NativeEventEmitter,
  NativeModules,
  SafeAreaView,
  Text,
  View,
} from "react-native";
import { useEffect, useState } from "react";
import {
  NORDIC_UART_SERVICE_UUID,
  RX_CHAR_UUID,
  SCAN_DURATION_SECONDS,
} from "./constants";
import { NavigationContainer } from "@react-navigation/native";

const BleManagerModule = NativeModules.BleManager;
const bleManagerEmitter = new NativeEventEmitter(BleManagerModule);

function App(): JSX.Element {
  const [peripherals, setPeripherals] = useState<Peripheral[]>([]);
  useEffect(() => {
    BleManager.start().then(scan);
  }, []);

  const handleDiscoverPeripheral = (peripheral: Peripheral) => {
    if (!peripheral.name) {
      peripheral.name = "NO NAME";
    }
    setPeripherals([peripheral, ...peripherals]);
  };

  const listeners = [
    bleManagerEmitter.addListener(
      "BleManagerDiscoverPeripheral",
      handleDiscoverPeripheral
    ),
  ];

  function removeListeners() {
    listeners.forEach((l) => {
      l.remove();
    });
  }

  function scan() {
    BleManager.scan([NORDIC_UART_SERVICE_UUID], SCAN_DURATION_SECONDS, false);
  }

  const renderDeviceItem = ({ item }: ListRenderItemInfo<Peripheral>) => {
    return (
      <View style={{ height: 50 }} id={item.id}>
        <Text>
          {item.name}, {item.id}
        </Text>
      </View>
    );
  };

  function stringToByteArray(str: string): number[] {
    let byteArray = new Uint8Array(str.length);
    for (let i = 0; i < str.length; i++) {
      byteArray[i] = str.charCodeAt(i) & 0xff;
    }
    return Array.from(byteArray);
  }

  return (
    <NavigationContainer>
      <SafeAreaView>
        <View>
          <Button
            title="Scan"
            onPress={() => {
              scan();
            }}
          />
          <Button
            title="Send"
            onPress={async () => {
              console.info(`connecting to ${peripherals[0].id}...`);
              await BleManager.connect(peripherals[0].id);
              const peripheralData = await BleManager.retrieveServices(
                peripherals[0].id
              );
              await BleManager.write(
                peripherals[0].id,
                NORDIC_UART_SERVICE_UUID,
                RX_CHAR_UUID,
                // Transmit value for "Black Beauty 8B"
                stringToByteArray("l#S175,P85,P152,E126#")
              );
            }}
          />
          <Text>{JSON.stringify(peripherals)}</Text>
          <FlatList
            style={{ height: "100%" }}
            data={peripherals}
            renderItem={renderDeviceItem}
          />
        </View>
      </SafeAreaView>
    </NavigationContainer>
  );
}

export default App;

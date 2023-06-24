import React from 'react';
import BleManager, {Peripheral} from 'react-native-ble-manager';
import {
  Button,
  FlatList,
  ListRenderItemInfo,
  NativeEventEmitter,
  NativeModules,
  SafeAreaView,
  Text,
  View,
} from 'react-native';
import {useEffect, useState} from 'react';

const BleManagerModule = NativeModules.BleManager;
const bleManagerEmitter = new NativeEventEmitter(BleManagerModule);

const NORDIC_UART_SERVICE_UUID = '6E400001-B5A3-F393-E0A9-E50E24DCCA9E';
const RX_CHAR_UUID = '6E400002-B5A3-F393-E0A9-E50E24DCCA9E';
const SCAN_DURATION_SECONDS = 10;

function App(): JSX.Element {
  const [peripherals, setPeripherals] = useState<Peripheral[]>([]);
  useEffect(() => {
    BleManager.start().then(() =>
      BleManager.scan([NORDIC_UART_SERVICE_UUID], SCAN_DURATION_SECONDS, false),
    );
  }, []);

  const handleDiscoverPeripheral = (peripheral: Peripheral) => {
    if (!peripheral.name) {
      peripheral.name = 'NO NAME';
    }
    setPeripherals([peripheral, ...peripherals]);
  };

  const listeners = [
    bleManagerEmitter.addListener(
      'BleManagerDiscoverPeripheral',
      handleDiscoverPeripheral,
    ),
  ];

  function removeListeners() {
    listeners.forEach(l => {
      l.remove();
    });
  }

  const renderDeviceItem = ({item}: ListRenderItemInfo<Peripheral>) => {
    return (
      <View style={{height: 50}} id={item.id}>
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
    <SafeAreaView>
      <View>
        <Button
          title="Send"
          onPress={async () => {
            console.info(`connecting to ${peripherals[0].id}...`)
            await BleManager.connect(peripherals[0].id);
            const peripheralData = await BleManager.retrieveServices(peripherals[0].id);
            await BleManager.write(
              peripherals[0].id,
              NORDIC_UART_SERVICE_UUID,
              RX_CHAR_UUID,
              stringToByteArray('hello world'),
            );
          }}
        />
        <Text>{JSON.stringify(peripherals)}</Text>
        {/* <FlatList
          style={{height: '100%'}}
          data={peripherals}
          renderItem={renderDeviceItem}
        /> */}
      </View>
    </SafeAreaView>
  );
}

export default App;

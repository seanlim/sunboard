import React, { useEffect, useState } from "react";
import {
  Button,
  FlatList,
  ListRenderItemInfo,
  NativeEventEmitter,
  NativeModules,
  Text,
  TouchableOpacity,
  View,
} from "react-native";

import {
  NORDIC_UART_SERVICE_UUID,
  RX_CHAR_UUID,
  SCAN_DURATION_SECONDS,
} from "./constants";
import BleManager, {
  BleDisconnectPeripheralEvent,
  BleManagerDidUpdateValueForCharacteristicEvent,
  BleScanCallbackType,
  BleScanMatchMode,
  BleScanMode,
  Peripheral,
} from "react-native-ble-manager";

const BleManagerModule = NativeModules.BleManager;
const bleManagerEmitter = new NativeEventEmitter(BleManagerModule);

export default function MainScreen(): JSX.Element {
  const [isScanning, setIsScanning] = useState(false);
  const [peripherals, setPeripherals] = useState(
    new Map<Peripheral["id"], Peripheral>()
  );

  console.debug("peripherals map updated", [...peripherals.entries()]);

  const addOrUpdatePeripheral = (id: string, updatedPeripheral: Peripheral) => {
    // new Map() enables changing the reference & refreshing UI.
    // TOFIX not efficient.
    setPeripherals((map) => new Map(map.set(id, updatedPeripheral)));
  };

  const handleStopScan = () => {
    setIsScanning(false);
    console.debug("[handleStopScan] scan is stopped.");
  };

  useEffect(() => {
    BleManager.start().then(scan);
  }, []);

  const scan = () => {
    if (!isScanning) {
      // reset found peripherals before scan
      setPeripherals(new Map<Peripheral["id"], Peripheral>());

      try {
        console.debug("[startScan] starting scan...");
        setIsScanning(true);
        BleManager.scan(
          [NORDIC_UART_SERVICE_UUID],
          SCAN_DURATION_SECONDS,
          false,
          {
            matchMode: BleScanMatchMode.Sticky,
            scanMode: BleScanMode.LowLatency,
            callbackType: BleScanCallbackType.AllMatches,
          }
        )
          .then(() => {
            console.debug("[startScan] scan promise returned successfully.");
          })
          .catch((err) => {
            console.error("[startScan] ble scan returned in error", err);
          });
      } catch (error) {
        console.error("[startScan] ble scan error thrown", error);
      }
    }
  };

  const handleDiscoverPeripheral = (peripheral: Peripheral) => {
    console.debug("[handleDiscoverPeripheral] new BLE peripheral=", peripheral);
    if (!peripheral.name) {
      peripheral.name = "NO NAME";
    }
    addOrUpdatePeripheral(peripheral.id, peripheral);
  };

  const handleDisconnectedPeripheral = (
    event: BleDisconnectPeripheralEvent
  ) => {
    let peripheral = peripherals.get(event.peripheral);
    if (peripheral) {
      console.debug(
        `[handleDisconnectedPeripheral][${peripheral.id}] previously connected peripheral is disconnected.`,
        event.peripheral
      );
      addOrUpdatePeripheral(peripheral.id, { ...peripheral });
    }
    console.debug(
      `[handleDisconnectedPeripheral][${event.peripheral}] disconnected.`
    );
  };

  const handleUpdateValueForCharacteristic = (
    data: BleManagerDidUpdateValueForCharacteristicEvent
  ) => {
    console.debug(
      `[handleUpdateValueForCharacteristic] received data from '${data.peripheral}' with characteristic='${data.characteristic}' and value='${data.value}'`
    );
  };

  const listeners = [
    bleManagerEmitter.addListener(
      "BleManagerDiscoverPeripheral",
      handleDiscoverPeripheral
    ),
    bleManagerEmitter.addListener("BleManagerStopScan", handleStopScan),
    bleManagerEmitter.addListener(
      "BleManagerDisconnectPeripheral",
      handleDisconnectedPeripheral
    ),
    bleManagerEmitter.addListener(
      "BleManagerDidUpdateValueForCharacteristic",
      handleUpdateValueForCharacteristic
    ),
  ];

  function removeListeners() {
    listeners.forEach((l) => {
      l.remove();
    });
  }

  function stringToByteArray(str: string): number[] {
    let byteArray = new Uint8Array(str.length);
    for (let i = 0; i < str.length; i++) {
      byteArray[i] = str.charCodeAt(i) & 0xff;
    }
    return Array.from(byteArray);
  }

  async function connectAndTransmit(id: string) {
    console.info(`connecting to ${id}...`);
    await BleManager.connect(id);
    const peripheralData = await BleManager.retrieveServices(id);
    await BleManager.write(
      id,
      NORDIC_UART_SERVICE_UUID,
      RX_CHAR_UUID,
      // Transmit value for "Black Beauty 8B"
      stringToByteArray("l#S175,P85,P152,E126#")
    );
  }

  if (isScanning) {
    return (
      <View style={{ flex: 1, alignItems: "center", justifyContent: "center" }}>
        <Text>Scanning...</Text>
      </View>
    );
  }

  const renderDeviceItem = ({ item }: ListRenderItemInfo<Peripheral>) => {
    return (
      <TouchableOpacity onPress={() => connectAndTransmit(item.id)}>
        <View
          style={{
            height: 50,
            backgroundColor: "white",
            flex: 1,
            justifyContent: "center",
            padding: 15,
          }}
          id={item.id}
        >
          <Text>{item.name}</Text>
        </View>
      </TouchableOpacity>
    );
  };

  return (
    <View style={{ flex: 1 }}>
      <FlatList
        style={{ height: "100%", flex: 1 }}
        data={Array.from(peripherals.values())}
        renderItem={renderDeviceItem}
        ListEmptyComponent={() => (
          <>
            <Text>No devices Found</Text>
            <Button title="Scan" onPress={scan} />
          </>
        )}
      />
    </View>
  );
}

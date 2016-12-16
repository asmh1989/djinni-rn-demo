/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 * @flow
 */

import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Text,
  View
} from 'react-native';

var stml = require('./js/stml');

export default class RNSmobiler extends Component {

  constructor()
  {
    super();
  }

  StmlEventConnecting(obj){

  }

  StmlEventConnected(obj){
    console.log('StmlEventConnected...');

    stml.post(`<Session ID="8E3A321F_4812_4020_B28D_6BA2B90F45B3"><Connect User="chiyu" Mode="Request" Form="" DeviceOS="IOS" DeviceOSVersion="10.1.1" DeviceVersion="3.5.0" Version="0.0.1" Device="iPhone7,1" ServiceKey=""/></Session>`);

  }

  StmlEventDisconnect(obj){
    console.log('StmlEventDisconnect...');

  }

  StmlEventSending(obj){

  }

  StmlEventSended(obj){

  }

  StmlEventReceived(obj){

    console.log('StmlEventReceived...');

    console.log(`${obj.msg}`);

  }

  componentWillMount(){
    console.log('componentWillMount...');

    stml.stmlEvent = {
      StmlEventConnecting:this.StmlEventConnecting.bind(this),
      StmlEventConnected:this.StmlEventConnected.bind(this),
      StmlEventDisconnect:this.StmlEventDisconnect.bind(this),
      StmlEventSending:this.StmlEventSending.bind(this),
      StmlEventSended:this.StmlEventSended.bind(this),
      StmlEventReceived:this.StmlEventReceived.bind(this)
    };

    stml.start('192.168.7.59', 23);
  }

  render() {
    return (
      <View style={styles.container}>
        <Text style={styles.welcome}>
          Welcome to React Native!
        </Text>
        <Text style={styles.instructions}>
          To get started, edit index.ios.js
        </Text>
        <Text style={styles.instructions}>
          Press Cmd+R to reload,{'\n'}
          Cmd+D or shake for dev menu
        </Text>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#F5FCFF',
  },
  welcome: {
    fontSize: 20,
    textAlign: 'center',
    margin: 10,
  },
  instructions: {
    textAlign: 'center',
    color: '#333333',
    marginBottom: 5,
  },
});

AppRegistry.registerComponent('RNSmobiler', () => RNSmobiler);

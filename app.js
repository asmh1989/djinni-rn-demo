/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 * @flow
 */

import React, { Component } from 'react';
import {
  StyleSheet,
  Text,
  View,
  Modal,
  Platform,
} from 'react-native';

import * as Progress from 'react-native-progress';
var DeviceInfo = require('react-native-device-info');


var stml = require('./js/stml');
var viewMaker = require('./js/viewMaker');

export default class RNSmobiler extends Component {

  state: {
    progressVisible: boolean,
    hasVtView: number,
    progressText: string,
  };


  constructor()
  {
    super();

    this.state = {
      progressVisible: false,
      hasVtView: 0,
      progressText:'请稍后..'
    };
  }

  StmlEventConnecting(obj: Object){

  }

  StmlEventConnected(obj: Object){
    console.log('StmlEventConnected...');

    stml.post(`<Session ID=\"${DeviceInfo.getUniqueID()}\"><Connect User="chiyu" Mode="Request" Form="" DeviceOS="${Platform.OS == 'ios' ? 'ios' : 'android'}" DeviceOSVersion="${DeviceInfo.getSystemVersion()}" DeviceVersion="3.5.0" Version="0.0.1" Device="${DeviceInfo.getDeviceId()}" ServiceKey=""/></Session>`);

    this.setState({
      progressVisible: true,
      hasVtView: 0,
      progressText:'数据传输中'

    })
  }

  StmlEventDisconnect(obj: Object){
    console.log('StmlEventDisconnect...');

    this.setState({
      progressVisible: false,
      hasVtView: 0,
      progressText:''
    });
  }

  StmlEventSending(obj: Object){
    let oldState = this.state;
    oldState.progressVisible = true;
    oldState.progressText = '请稍后..';
    this.setState(oldState);
  }

  StmlEventSended(obj: Object){

  }

  StmlEventReceived(obj: Object){
    console.log(`StmlEventReceived: ${obj.msg}`);

    let oldState = this.state;
    oldState.progressVisible = false;
    let msg = JSON.parse(obj.msg);
    if(msg.Type == 'end'){
      this.setState({
        progressVisible: false,
        hasVtView: this.state.hasVtView+1,
        progressText:''
      });
    } else {
        viewMaker.create(msg);
        // this.setState(oldState);
    }
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

    this.setState({
      progressVisible: true,
      hasVtView: 0,
      progressText: '正在连接..'
    });

  }

  render() {

    let time = new Date();

    let progressKey = `progressView${time.toLocaleTimeString()}`;
    let progressView = <Modal
      animationType={"fade"}
      transparent={true}
      visible={this.state.progressVisible}
      key={progressKey}
      onRequestClose={() => {}}
    >
      <View style={[styles.container, {"backgroundColor": 'rgba(0,0,0, 0.3)'}]} >
        <View style={styles.modalStyle}>
          <Progress.Circle style={{ justifyContent: 'center' }} size={40} indeterminate={true} />
          <Text style={{
              fontSize: 16,
              textAlign: 'center',
              margin: 10,
              color:'#888888'
            }}>{this.state.progressText}</Text>
        </View>
      </View>
    </Modal>;

    if(this.state.hasVtView > 0){
      let views = viewMaker.elements;
      views.push(progressView);
      let mainView = React.createElement(View, {style: {flex: 1}}, views);

      return mainView;
    }
    else
      return (
      <View style={styles.container}>
        <Text style={styles.welcome}>
          Welcome to RNSmobiler!
        </Text>
        {progressView}
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
  modalStyle: {
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#333333',
    borderRadius: 10,
    padding: 12,
  }
});

/**
 * Created by sun on 2016/12/16.
 */
'use strict';


import {
  NativeModules,
  NativeEventEmitter
} from 'react-native';

const StmlManager = NativeModules.StmlManager;

class Stml extends NativeEventEmitter
{
  stmlEvent: ?Object;

  constructor() {
    super(StmlManager);

    this.stmlEvent = null;

    console.log(`${StmlManager.StmlEventConnecting}, ${StmlManager.StmlEventConnected}`);

    this.addListener(StmlManager.StmlEventConnecting, (obj) => {
        if(this.stmlEvent && this.stmlEvent.onConnecting)
        {
          this.stmlEvent.onConnecting(obj);
        }
    });

    this.addListener(StmlManager.StmlEventConnected, (obj) => {
      console.log('StmlManager.StmlEventConnected...');

      if(this.stmlEvent && this.stmlEvent.StmlEventConnected)
      {
        this.stmlEvent.StmlEventConnected(obj);
      }
    });

    this.addListener(StmlManager.StmlEventDisconnect, (obj) => {
      if(this.stmlEvent && this.stmlEvent.StmlEventDisconnect)
      {
        this.stmlEvent.StmlEventDisconnect(obj);
      }
    });

    this.addListener(StmlManager.StmlEventSending, (obj) => {
      if(this.stmlEvent && this.stmlEvent.StmlEventSending)
      {
        this.stmlEvent.StmlEventSending(obj);
      }
    });

    this.addListener(StmlManager.StmlEventSended, (obj) => {
      if(this.stmlEvent && this.stmlEvent.StmlEventSended)
      {
        this.stmlEvent.StmlEventSended(obj);
      }
    });

    this.addListener(StmlManager.StmlEventReceived, (obj) => {
      if(this.stmlEvent && this.stmlEvent.StmlEventReceived)
      {
        this.stmlEvent.StmlEventReceived(obj);
      }
    });
  }

  StmlEventConnected(obj){
    console.log('StmlManager.StmlEventConnected');
    if(this.stmlEvent && this.stmlEvent.StmlEventConnected)
    {
      this.stmlEvent.StmlEventConnected(obj);
    }
  }


  start(host: string, port: number)
  {
    StmlManager.start(host, port);
  }

  post(msg: string)
  {
    StmlManager.post(msg);
  }
}

Stml = new Stml();

module.exports = Stml;

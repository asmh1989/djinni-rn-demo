/**
 * Created by sun on 2016/12/19.
 */

import React, { Component } from 'react';
import {
  StyleSheet,
  View,
  Dimensions,
  Text,
  Button
} from 'react-native';
var DeviceInfo = require('react-native-device-info');

var stml = require('./stml');

class VTLabel extends Component
{
  state: {
    style:Object;
  };
  constructor(props)
  {
    super(props)
    this.state = {style:{}};
  }

  render() {
    console.log(`props : ${JSON.stringify(this.props)}`);
    return ( <Text style={[this.props.styles, {position: 'absolute'}]} >{this.props.text}</Text>);
  }

}

class VTButton extends Component
{
  state: {
    style:Object;
  };
  constructor(props)
  {
    super(props)
    this.state = {style:{}};
  }

  onClick(){
    stml.post(`<Session ID="${DeviceInfo.getUniqueID()}"><Event Name="${this.props.name}" EventType="Click" EventParams="" /></Session>`);
  }

  render() {
    // console.log(`props : ${JSON.stringify(this.props)}`);
    return (
      <View style={[this.props.styles, {position: 'absolute'}] }>
        <Button  title={this.props.text} onPress={this.onClick.bind(this)} />
      </View>
    );
  }

}

class ViewMarker
{
  elements: Array;
  Form:{
    width: number,
    height: number,
  };
  scale: number;

  constructor()
  {
    this.Form = {
      width:0,
      height:0
    };

    this.scale = 1;

    this.elements = new Array;
  }

  create(obj: Object)
  {
    if(obj.Type == 'Form'){
      while(this.elements.length > 0){
        this.elements.pop();
      }

      this.Form.width = Number.parseInt(obj.Width);
      this.Form.height = Number.parseInt(obj.Height);

      if(obj.ScreenOrientation == 'Landscape'){
        this.scale = Dimensions.get('window').width / 300;
      } else {
        this.scale = Dimensions.get('window').width / this.Form.width;
      }

    } else if(obj.Type == 'Label'){
      let style = new Object;
      // style.Position='Absolute';
      let attrs = Object.keys(obj);
      let text = '';
      let key='';
      for(let i in attrs)
      {
        let attr = attrs[i];
        if(attr == 'Left'){
          style.left = Number.parseInt(obj[attr]) * this.scale;
        } else if ( attr == 'Top') {
          style.top = Number.parseInt(obj[attr]) * this.scale;
        } else if ( attr == 'Width') {
          style.width = Number.parseInt(obj[attr]) * this.scale;
        } else if( attr == 'Height') {
          style.height = Number.parseInt(obj[attr]) * this.scale;
        } else if( attr == 'Text'){
          text = obj[attr];
        } else if( attr == 'Name'){
          key = obj[attr];
        } else if( attr == 'FontSize'){
          style.fontSize = Number.parseInt(obj[attr]);
        } else if( attr == 'HorizontalAlignment'){
          if(obj[attr] == 'Center'){
            style.textAlign = 'center';
          } else if(obj[attr] == 'Right'){
            style.textAlign = 'right';
          }
        }
      }

      console.log(`style=${JSON.stringify(style)}`);
      let view = <VTLabel styles={style} key={key} text={text} name={key} />
      this.elements.push(view);
    } else if(obj.Type == 'Button'){
      let style = new Object;
      // style.Position='Absolute';
      let attrs = Object.keys(obj);
      let text = '';
      let key='';
      for(let i in attrs)
      {
        let attr = attrs[i];
        if(attr == 'Left'){
          style.left = Number.parseInt(obj[attr]) * this.scale;
        } else if ( attr == 'Top') {
          style.top = Number.parseInt(obj[attr]) * this.scale;
        } else if ( attr == 'Width') {
          style.width = Number.parseInt(obj[attr]) * this.scale;
        } else if( attr == 'Height') {
          style.height = Number.parseInt(obj[attr]) * this.scale;
        } else if( attr == 'Text'){
          text = obj[attr];
        } else if( attr == 'Name'){
          key = obj[attr];
        }
      }

      console.log(`style=${JSON.stringify(style)}`);
      let view = <VTButton styles={style} key={key} name={key} text={text} />
      this.elements.push(view);
    }
  }


}

maker = new ViewMarker();

module.exports = maker;

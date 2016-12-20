/**
 * Created by sun on 2016/12/19.
 */

import React, { Component } from 'react';
import {
  StyleSheet,
  View,
  Dimensions,
  Text
} from 'react-native';

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
      this.Form.width = Number.parseInt(obj.Width);
      this.Form.height = Number.parseInt(obj.Height);

      this.scale = Dimensions.get('window').width / this.Form.width;

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
      let view = <VTLabel styles={style} key={key} text={text} />
      this.elements.push(view);
    }
  }


}

maker = new ViewMarker();

module.exports = maker;

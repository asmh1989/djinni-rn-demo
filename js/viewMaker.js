/**
 * Created by sun on 2016/12/19.
 */
import React, { Component } from 'react';
import {
	StyleSheet,
	View,
	Dimensions,
	Text,
	Button,
	TextInput,
	WebView,
	Image
} from 'react-native';

var stml = require('./stml');
var DeviceInfo = require('react-native-device-info');

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

class VTImage extends Component
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
		return ( <Image style={[this.props.styles, {position: 'absolute'}]} source={{uri: this.props.uri}} resizeMode={this.props.resizeMode} />);
	}

}

class VTWebView extends Component
{
	state: {
		style:Object;
	};
	constructor(props)
	{
		super(props)
		this.state = {style:{}};
	}

	componentWillMount()
	{
		console.log(`componentWillMount, VTWebView ...`);
	}

	componentWillUnmount()
	{
		console.log(`componentWillUnmount, VTWebView ...`);
	}

	render() {
		console.log(`props : ${JSON.stringify(this.props)}`);
		return (
			<View style={[this.props.styles, {position: 'absolute'}] }>

				<WebView source={{uri: this.props.uri}}  />
			</View>);
	}

}

class VTTextInput extends Component
{
	state: {
		style:Object;
		text:string;
	};
	constructor(props)
	{
		super(props)
		this.state = {style:{}, text:''};
	}

	render() {
		console.log(`props : ${JSON.stringify(this.props)}`);
		return ( <TextInput style={[this.props.styles, {position: 'absolute'}]}
												value={this.state.text.length == 0 ? this.props.text : this.state.text}
												onChangeText={(text) => {
      let oldState = this.state;
      this.state.text = text;
      this.setState(oldState);
    }}
												placeholder={this.props.placeholder}/>);
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
		console.log(`props : ${JSON.stringify(this.props)}`);
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
				this.scale = Dimensions.get('window').width / 360;
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

			// console.log(`style=${JSON.stringify(style)}`);
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

			// console.log(`style=${JSON.stringify(style)}`);
			let view = <VTButton styles={style} key={key} name={key} text={text} />
			this.elements.push(view);
		} else if( obj.Type =='TextBox' ){
			let style = new Object;
			// style.Position='Absolute';
			let attrs = Object.keys(obj);
			let text = '';
			let key='';
			let placeholder='';
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
				} else if( attr == 'BorderRadius'){
					style.borderRadius = Number.parseInt(obj[attr]);
				} else if( attr == 'WaterMarkText'){
					placeholder = obj[attr];
				} else if( attr == 'HorizontalAlignment'){
					if(obj[attr] == 'Center'){
						style.textAlign = 'center';
					} else if(obj[attr] == 'Right'){
						style.textAlign = 'right';
					}
				}
			}

			style.borderColor='#000000';
			style.borderWidth=1;

			// console.log(`style=${JSON.stringify(style)}`);
			let view = <VTTextInput styles={style} key={key} name={key} text={text} placeholder={placeholder}/>
			this.elements.push(view);
		} else if(obj.Type =='WebView'){
			let style = new Object;
			// style.Position='Absolute';
			let attrs = Object.keys(obj);
			let key='';
			let uri=''
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
				} else if( attr == 'Name'){
					key = obj[attr];
				} else if( attr == 'Url'){
					uri = 'https://'+obj[attr];
				}
			}

			// console.log(`style=${JSON.stringify(style)}`);
			let view = <VTWebView styles={style} key={key} uri={uri} name={key} />
			this.elements.push(view);
		} else if( obj.Type == 'ImageButton'){
			let style = new Object;
			// style.Position='Absolute';
			let attrs = Object.keys(obj);
			let key='';
			let uri='';
			let resizeMode='contain';
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
				} else if( attr == 'Name'){
					key = obj[attr];
				} else if( attr == 'ImageID'){
					uri = 'http://192.168.7.59:2324'+obj[attr];
				} else if( attr == 'SizeMode'){
					resizeMode=obj[attr];
				}
			}

			// console.log(`style=${JSON.stringify(style)}`);
			let view = <VTImage styles={style} key={key} uri={uri} name={key}  resizeMode={resizeMode}/>
			this.elements.push(view);
		}
	}
}

maker = new ViewMarker();

module.exports = maker;

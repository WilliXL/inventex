import React, { Component } from 'react';
import {
    Image,
    Text,
    View,
    StyleSheet,
    Picker,
    ActivityIndicator,
    TouchableOpacity,
    Button } from 'react-native';
import {
    ImagePicker,
    Constants } from 'expo';

export default class App extends Component {
    state = {
        selectedPart: null,
        parts: [],
        uploading: false
    };

    componentWillMount() {
        this.updateParts();
    }

    updateParts = async () => {
        let apiUrl = 'http://192.168.43.61:5000/parts';
        let options = {
            method: 'GET',
            headers: {
                'Content-Type': "application/json",
            }
        };
        let uploadResponse, uploadResult;
        try {
            uploadResponse = await fetch(apiUrl, options);
            uploadResult = await uploadResponse.json();
            this.setState({parts: uploadResult});
            console.log('PARTS RETRIEVED!')
        } catch (e) {
            console.log({uploadResponse});
            console.log({uploadResult});
            console.log({e});
            console.log('FAILED in parts retrieval')
        } finally {
            if (this.state.parts.length == 1) {
                this.state.selectedPart = this.state.parts[0];
            }
        }
    };

    render() {
        let { parts } = this.state;
        let partItems = [];
        for (var i = 0; i < parts.length; i++) {
            var name = `Bin ${i+1}: ${parts[i]}`
            partItems.push(<Picker.Item key={i} label={name} value={parts[i]}/>);
        }
        return (
            <View style={styles.container}>
                <View style={styles.logoContainer}>
                    <Image source={require("./assets/invent.jpg")}/>
                </View>
                <View style={styles.pickerContainer}>
                    <View style={{flex: 1,marginLeft:30}}>
                        <Picker prompt="choose a part" mode='dropdown' style={[styles.picker]} itemStyle={styles.pickerItem}
                            selectedValue={this.state.selectedPart}
                            onValueChange={(itemValue,itemIndex) => {
                                this.setState({selectedPart: itemValue});
                            }}>
                          {partItems}
                        </Picker>
                        <Text style={{color: "white"}}>Select a part</Text>
                    </View>
                    <View style={styles.outputContainer}>
                        <TouchableOpacity onPress={this._retrievePart}>
                            <Image source={require("./assets/out.png")}/>
                        </TouchableOpacity>
                    </View>
                </View>

                <View style={styles.inputContainer}>
                    <TouchableOpacity onPress={this._captureImage}>
                        <Image source={require("./assets/in.png")}/>
                    </TouchableOpacity>
                </View>
                <View style={styles.buttonContainer}>
                    <Button
                        title="Reset"
                        color={"black"}
                        onPress={this._reset}
                    />
                </View>
                {this._maybeRenderUploadingOverlay()}
            </View>
        );
    }

    _reset = async () => {
        let apiUrl = "http://192.168.43.61:5000/reset";
        let options = {
            method: "POST"
        };
        let uploadResponse, uploadResult;
        try {
            uploadResponse = await fetch(apiUrl, options);
            uploadResult = await uploadResponse.json();
            console.log(uploadResult);
            alert('Reset was successful!')
        } catch (e) {
            console.log({uploadResponse});
            console.log({uploadResult});
            console.log({e});
            alert('Reset failed')
        } finally {
            this.updateParts();
            if (this.state.parts == []) {
                this.setState({selectedPart: null})
            }
        }
    };

    _maybeRenderUploadingOverlay = () => {
        if (this.state.uploading) {
            return (
                <View
                    style={[
                        StyleSheet.absoluteFill,
                        {
                            backgroundColor: 'rgba(0,0,0,0.4)',
                            alignItems: 'center',
                            justifyContent: 'center',
                        },
                    ]}>
                    <ActivityIndicator color="#fff" animating size="large" />
                </View>
            );
        }
    };

    _retrievePart = async () => {
        if (this.state.selectedPart == null) {
            alert('Select a part first!');
            return
        }
        let apiUrl = 'http://192.168.43.61:5000/retrieve';
        var data = {part: this.state.selectedPart};
        let options = {
            method: 'POST',
            body: JSON.stringify(data),
            headers: {
                'Accept': 'application/json',
                'Content-Type': "application/json"
            }
        };
        console.log({options});
        let uploadResponse, uploadResult;
        try {
            uploadResponse = await fetch(apiUrl, options);
            uploadResult = await uploadResponse.json();
            console.log(uploadResult);
            alert('Retrieval was successful!')
        } catch (e) {
            console.log({uploadResponse});
            console.log({uploadResult});
            console.log({e});
            alert('Retrieval failed')
        }

    };

    _captureImage = async () => {
        let result = await ImagePicker.launchCameraAsync({
            quality: 1.0,
        });

        this._handleImagePicked(result)
    };

    _handleImagePicked = async (pickerResult) => {
        let uploadResponse, uploadResult;

        try {
            this.setState({uploading: true});

            if (!pickerResult.cancelled) {
                uploadResponse = await this.uploadImageAsync(pickerResult.uri);
                uploadResult = await uploadResponse.json();
                alert('This was recognized as a ' + uploadResult["part"]);
                this.updateParts();
            }
        } catch(e) {
            console.log({uploadResponse});
            console.log({uploadResult});
            console.log({e});
            alert('Upload failed, sorry :(');
        } finally {
            this.setState({uploading: false});
        }
    };

    uploadImageAsync = async (uri) => {
        let apiUrl = 'http://192.168.43.61:5000/upload';

        let formData = new FormData();
        formData.append('file', {
            uri,
            name: `photo.jpg`,
            type: `image/jpg`,
        });

        let options = {
            method: 'POST',
            body: formData,
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'multipart/form-data',
            },
        };

        return fetch(apiUrl, options);
    }
}

const styles = StyleSheet.create({
    container: {
        flex: 1,
        display: "flex",
        backgroundColor: 'black',
        alignItems: "center",
        justifyContent: "center"
    },

    inputContainer: {
    },

    outputContainer: {
        flex: 1
    },

    rowedView: {
        flexDirection: 'row',
        flexWrap: 'wrap'
    },

    pickerContainer: {
        flexDirection: 'row',
        backgroundColor: 'black',
        alignItems: "center",
        justifyContent: "center",
    },

    picker: {
        marginTop: -100,
        backgroundColor: 'white',
        color:'black',
    },

    pickerItem: {
        height: 44,
        color: 'white',
        backgroundColor: 'black',
    },
    buttonContainer: {
    },

    logoContainer: {
        paddingBottom: 40,
        marginBottom:40,
        borderBottomColor: "white",
        borderWidth: 5,
        marginTop: 20
    }
});
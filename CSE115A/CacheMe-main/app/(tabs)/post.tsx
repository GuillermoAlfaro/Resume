import React, { useState } from 'react';
import { Image, View, StyleSheet, TouchableOpacity, Dimensions, TextInput, Alert, KeyboardAvoidingView, TouchableWithoutFeedback, Keyboard } from 'react-native';
import * as ImagePicker from 'expo-image-picker';
import { ref, uploadBytes } from "firebase/storage"
import {storage, db } from "../../firebase"
import uuid from 'react-native-uuid';
import * as ImageManipulator from 'expo-image-manipulator';
import { ImageResult } from 'expo-image-manipulator';
import BigText from '../../components/Texts/bigText';
import RegularText from '../../components/Texts/regularText';
import MiniText from '../../components/Texts/miniText';
import { collection, GeoPoint, doc, setDoc, Timestamp } from "firebase/firestore";
import SelectPrivacyScreen from '../../components/dropDownPrivacy';
import SelectRadiusScreen from '../../components/dropDownRadius';
import { FontAwesome } from '@expo/vector-icons';
import { auth } from '../../firebase'
import getLocation from '../../helpers/location'

import {getDoc} from 'firebase/firestore';
import getProfileImage from "../../helpers/profile";



import { useEffect} from 'react';

import {  useRouter } from 'expo-router';


type UserData = {
  __id: string;
  email: string;
  name: string;
  profilePicture: string;
  username: string;
  friends: [];
}



const windowWidth = Dimensions.get('screen').width
const windowHeight = Dimensions.get('screen').height



export default function Post() {
  const router = useRouter();
    const [imageURI, setImageURI] = useState("");
    const [image, setImage] = useState<ImageResult>();
    const [caption, setCaption] = useState("");
    let imageID = uuid.v1().toString();
    const [captionExists, setCaptionExists] = useState(false);
    const [posting, setPosting] = useState(false);
    const postCache = async () => {
      setPosting(true);
      if(await uploadImage()){
        addCacheToFirestore();
      }
      setImageURI("");
      setImage(undefined);
      setCaption("");
      setCaptionExists(false);
      setPosting(false);
      router.push("/feed");
      
    }

    const [pfpURI, setpfpURI] = useState("");

    const [user, setUser] =  useState<UserData>();
    const [profilePictureID, setProfilePictureID] = useState<string>();
    
    useEffect (() => {
      const fetchUser = async (id: string) => {
          const user: UserData = {
              __id:  "",
              email: "",
              name: "",
              profilePicture: "",
              username: "",
              friends: []
          }
          const userDoc = await getDoc(doc(db, "user", id));
          if (userDoc.exists()) {
            user.__id = userDoc.data().__id;
            user.email = userDoc.data().email;
            user.name = userDoc.data().name;
            user.profilePicture = userDoc.data().profilePicture;
            user.username = userDoc.data().username;
            user.friends = userDoc.data().friends;
            setProfilePictureID(user.profilePicture);
          }
          setUser(user);
      }
      fetchUser(auth.currentUser?.uid ? auth.currentUser?.uid : "");
  }, [auth.currentUser?.uid]) 

    
    if(profilePictureID){
      getProfileImage(profilePictureID as string).then( async (uri) => {
        if (uri) {
            setpfpURI(uri);
        } else {
            setpfpURI("");
        }
   })};


    const addCacheToFirestore = async () => {
      try {
        await getLocation().then((location) => {
        const newDocRef = doc(collection(db, "cache"));
        setDoc(
              newDocRef, 
              {
                __id: newDocRef.id,
                __imageId: image ? imageID : "",
                __userId: auth.currentUser?.uid,
                _createdAt: Timestamp.fromDate(new Date()),
                caption: caption,
                comments: [],
                location: new GeoPoint(location?.coords.latitude as number, location?.coords.longitude as number),
                numComments: 0,
                numLikes: 0,
                likeIDs: [],
                reported: false
              }
            )
        console.log("Document written with ID: ", newDocRef.id);
            });
      } catch (e) {
        console.error("Error adding document: ", imageID);
      }
    }

    const pickImage = async () => {
        let result = await ImagePicker.launchImageLibraryAsync({
          mediaTypes: ImagePicker.MediaTypeOptions.All,
          allowsEditing: true,
          aspect: [4, 3],
          quality: 1,
          base64: true, // Remove?
        });
        
        if (!result.canceled) {
          setImageURI(result.assets[0].uri);
          setImage(await ImageManipulator.manipulateAsync(result.assets[0].uri, [{resize: {width: 500}}], {compress: 1}))
        }
      };

    const takeImage = async () => {
      await ImagePicker.requestCameraPermissionsAsync();
      let result = await ImagePicker.launchCameraAsync({
        allowsEditing: true,
        aspect: [4, 3],
        quality: 1
      });

      if (!result.canceled) {
        setImageURI(result.assets[0].uri);
        setImage(await ImageManipulator.manipulateAsync(result.assets[0].uri, [{resize: {width: 500}}], {compress: 1})) 
      }
    }

    const uploadImage = async () => { 
      if(image){
        const response = await fetch(image.uri);
        const blobFile = await response.blob();
        const imageRef = ref(storage, `images/${imageID}`);
        const result = await uploadBytes(imageRef, blobFile);
        if(result){
          Alert.alert("Upload successful");
          return true;
        }else{
          Alert.alert("Upload failed!");
          return false;
        }
      }
      return true;   
    }


    return(
      <TouchableWithoutFeedback onPress={Keyboard.dismiss}>
        <View style={styles.container}>
          <View style={styles.topRow}>
            <BigText>Create Cache</BigText>
            {!posting ? <TouchableOpacity style={styles.postButton} onPress={(image || captionExists)  ? postCache : () => Alert.alert("Add a picture or caption first!")}>
              <RegularText>Post</RegularText>
            </TouchableOpacity> : <TouchableOpacity style={styles.postButton}>
              <RegularText>Posting...</RegularText>
            </TouchableOpacity>}
          </View>

          <View style={styles.postSettings}>
            <View style={{
              width: windowWidth * .3,
              justifyContent: "center",
              alignItems: "center",
              }}> 
                {pfpURI != '' && 
                <Image source={{uri: pfpURI}} 
                style={{
                    width: windowWidth*.15,
                    height: windowWidth*.15,
                    borderRadius: (windowWidth*.15)/2
                }}
             />}
            </View>
            <View style={styles.dropDownMenu}>
              <MiniText>Who can see it?</MiniText>
              <SelectPrivacyScreen></SelectPrivacyScreen>
            </View>

            <View style={styles.dropDownMenu}>
              <MiniText>Post Discovery Radius:</MiniText>
              <SelectRadiusScreen></SelectRadiusScreen>
            </View>
          </View>
          <View>
            <TextInput 
              style={styles.textInput} 
              placeholder="What's your story?" 
              onChangeText={newText => {
                setCaption(newText);
                newText != "" ? setCaptionExists(true) : setCaptionExists(false);
              }}
              defaultValue={caption}
              placeholderTextColor="#B3B3B3"
              />
            
            {imageURI && <Image source={{ uri: imageURI }} style={styles.image} />}
          </View>
          <KeyboardAvoidingView style={styles.bottomRow}
          behavior='padding'>
            <TouchableOpacity onPress={pickImage} style={{padding: 8}}>
              <FontAwesome name="picture-o" size={24} color="black"  />
            </TouchableOpacity>
            <TouchableOpacity onPress={takeImage} style={{padding: 8}}>
              <FontAwesome name="camera" size={24} color="black"  />
            </TouchableOpacity>
          </KeyboardAvoidingView>
        </View>
        </TouchableWithoutFeedback>
    )
}



const styles = StyleSheet.create({
  container: {
    display: 'flex',
    minHeight: windowHeight - windowHeight/13,
    paddingTop: windowHeight/20,
    backgroundColor: "#EEF2FF",
  },
  topRow: {
    flexDirection: 'row',
    position: "relative",
    justifyContent: "center",
  },
  bottomRow: { 
    flexDirection: 'row',
    position: "relative",
    paddingHorizontal: windowWidth/15 ,
    marginBottom: 15,
    marginTop: "auto",
    
  },
  postButton: {
    right: 1,
    position: "absolute",
    display: 'flex',
    backgroundColor: "white",
    padding: 5,
    marginRight: windowWidth/25, 
    borderRadius: 10,
  },
  postSettings: {
    flexDirection: 'row',
    position: "relative",
    display: 'flex',
    justifyContent: "space-around",
  },
  dropDownMenu: {
    display: "flex",
    padding: 5,
    borderRadius: 10,
  },
  image: {
    flexDirection: 'row',
    position: "relative",
    width: windowWidth-75,
    height: windowWidth-75,
    alignSelf: "center",
    borderRadius: 10,
  },
  textInput:{
    height: 40,
    paddingLeft: windowWidth/12,
  }

})
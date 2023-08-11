import React, { useEffect, useState } from 'react'
import { View, StyleSheet, Image, Dimensions, TouchableOpacity } from 'react-native'
import { LinearGradient } from 'expo-linear-gradient';
import { AntDesign, FontAwesome, Ionicons } from '@expo/vector-icons';
import { } from '@expo/vector-icons';
import { useLocalSearchParams, useRouter } from 'expo-router';
import RegularText from "../Texts/regularText"
import { GeoPoint } from 'firebase/firestore';
import { auth } from '../../firebase';

import { doc, getDoc } from 'firebase/firestore';
import getProfileImage from "../../helpers/profile";
import { db } from '../../firebase';



const windowWidth = Dimensions.get('screen').width
const windowHeight = Dimensions.get('screen').height

type CacheData = {
    id: string, 
    userId: string, 
    username: string,
    imageRef: string, 
    caption: string, 
    distBtwn: number, 
    timePosted: string, 
    location: GeoPoint | null, 
    nLikes: number,
    nComments: number,
  }
  type UserData = {
    __id: string;
    email: string;
    name: string;
    profilePicture: string;
    username: string;
    friends: [];
  }

  

const PostHeader = (props: CacheData) => {
    const [outOfRadius, setOutOfRadius] = useState(true);
    const router = useRouter();
    const params = useLocalSearchParams();

    const viewProfile = () => {
        if (props.userId == auth.currentUser?.uid) {
            router.push({ pathname: 'profile' })
            return;
        }
        const { userId = props.userId } = params;
        router.push({ pathname: '/viewProfile', params: { userId }});
    }

    useEffect( () => {
            if (props.distBtwn > 5) {
                setOutOfRadius(true);
            } else {
                setOutOfRadius(false);
            }
    }, [props.distBtwn]);
    const [pfpURI, setpfpURI] = useState("");

    const [user, setUser] =  useState<UserData>();
    const [profilePictureID, setProfilePictureID] = useState<string>();
    
    useEffect (() => {
      const fetchUser = async (id: string) => {
        if(id == "" || id == null){
            return;
        }
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
      fetchUser(props.userId);
  }, [props.userId]) 

    if(profilePictureID){
      getProfileImage(profilePictureID as string).then( async (uri) => {
        if (uri) {
            setpfpURI(uri);
        } else {
            setpfpURI("");
        }
   })};

   return (
    <View style={styles.container}>
        <View style={styles.topContainer}>
            <TouchableOpacity style={styles.backArrow} onPress={router.back}>
                <Ionicons name="arrow-back-outline" size={35} color="black" />
            </TouchableOpacity>
            <View style={styles.profilePic}>
            {pfpURI != '' && 
                <Image source={{uri: pfpURI}} 
                style={{
                    width: windowWidth*.125,
                    height: windowWidth*.125,
                    borderRadius: (windowWidth*.125)/2,
                }}
             />}      
             </View>
            <TouchableOpacity style={styles.userName} onPress={viewProfile}>
                <RegularText>{ props.username }</RegularText>
            </TouchableOpacity>
            <View style={styles.time}>
                <RegularText style={{color: "#545350"}}>{ props.timePosted }</RegularText>
            </View>
        </View>
        <View style={styles.postBody}>
            {props.imageRef == null || props.imageRef == "" ? (
                <></>
            ) : (
                <View style={styles.image}> 
                    <Image source={{uri: props.imageRef}}
                    style={{
                            width: windowWidth, 
                            height: windowWidth
                        }}
                        blurRadius={outOfRadius ? 20 : 0}
                    />
                </View> 
            )}
            <View style={styles.caption}>
                <RegularText>{outOfRadius ? "Move closer to unlock this cache!" : props.caption }</RegularText>
            </View>
        </View>
        
        <LinearGradient
            colors={['rgba(44, 218, 157, .45)', 'rgba(4, 67, 137, .45)']}
            start={{ x: 0.1, y: 0.1 }}
            end={{ x: 0.9, y: 0.9 }}
            style={styles.interactBar}>
            <TouchableOpacity>
                <AntDesign name="heart" size={35} color={'#8E4162'} style={styles.icons}/>
            </TouchableOpacity>
            <RegularText style={{ marginLeft: 5}}>{ props.nLikes }</RegularText>
            <TouchableOpacity>
                <FontAwesome name="comment" size={35} color="white" style={{marginLeft: 10}}/>
            </TouchableOpacity>
            <RegularText style={{ marginLeft: 5}}>{ props.nComments }</RegularText>
            <RegularText style={{ marginLeft: 165, color: "#545350" }}>{ props.distBtwn } km away</RegularText>
            <TouchableOpacity>
                <Ionicons name="location-sharp" size={35} color="white" />
            </TouchableOpacity>
        </LinearGradient>
        
    </View>
    )
}

const styles = StyleSheet.create({
    container: {
        flex: 1,
        border: "flex",
        width: windowWidth,
        marginTop: windowHeight/25,
        justifyContent: "flex-start",
        alignContent: "flex-start",
        flexDirection: "column",
        // backgroundColor: "#EEF2FF"
        
    },
    topContainer: {
        flexDirection: "row",
        justifyContent: "space-between",
        alignItems: "center",
        height: windowHeight/15,
    },
    postBody: {
        flexDirection: 'column',
        justifyContent: 'space-between',
        alignItems: 'center',
        margin: 10,
    },
    backArrow: {
        //flex: 1,
        width: 50,
        height: 30,
        marginLeft: 8,
    },
    profilePic: {
        flex: 1,
        width: 50,
        height: 50,
        marginLeft: 60,
        position: 'absolute',
    },
    userName: {
        marginLeft: 115,
        position: 'absolute',
        justifyContent: "center",
    },
    time: {
        height: 30,
        justifyContent: "center",
        alignItems: "flex-end",
        paddingRight: 25,
    },
    image: {
        width: windowWidth,
        // height: windowWidth,
        justifyContent: "center",
        alignItems: "center",
    },
    caption: {
        padding:2,
        width: windowWidth,
        justifyContent: 'flex-start'
    },
    interactBar: {
        flexDirection: "row",
        width: windowWidth,
        height: 50,
        alignItems: 'center',
    },
    icons: {
        flexDirection: "row",
        paddingLeft: 15,
    },blurredText: {
        margin: 15,
        marginLeft: 20,
        marginRight: 20,
        justifyContent: "flex-start",
        height: 3,
        width: 70,
        shadowOpacity: 1,
        shadowColor: '#000',
        shadowOffset: { width: 10, height: 10 },
        shadowRadius: 5,
        elevation: 5,
        borderWidth: 0.5,
        borderColor: "white",
        backgroundColor: "rgba(255, 255, 255, 1)"
    }

})

export default PostHeader


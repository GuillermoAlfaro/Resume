// This file is used as a template for how a post should look.
import React, {useState, useEffect} from 'react'
import { useWindowDimensions, View, Image, StyleSheet, TouchableOpacity, Text, Dimensions} from 'react-native'
import { AntDesign } from '@expo/vector-icons';
import { FontAwesome } from '@expo/vector-icons';
import { Ionicons } from '@expo/vector-icons';
import RegularText from './Texts/regularText';
import { LinearGradient } from 'expo-linear-gradient';
import moment from 'moment';
import { useLocalSearchParams, useRouter } from 'expo-router';
import { doc, getDoc, updateDoc, GeoPoint, Timestamp, increment, arrayUnion, arrayRemove } from "firebase/firestore";
import { db, auth, storage } from '../firebase';;
import { getDownloadURL, ref } from "firebase/storage";
import {getUserDistanceFromPost} from '../helpers/post';

import getProfileImage from "../helpers/profile";


type CacheData = {
    id: string,
    captionText: string,
    uid: string, 
    image: string,
    numComments: number,
    numLikes: number,
    location: GeoPoint,
    timePosted: Timestamp,
    likeIDs: string[],
}

type UserData = {
    __id: string;
    email: string;
    name: string;
    profilePicture: string;
    username: string;
    friends: [];
}

const windowWidth = Dimensions.get('screen').width


const PostPreview = (props: CacheData) => {

    const {width} = useWindowDimensions()
    const [isLiked, setIsLiked] = useState(false);
    const [likeCount, setLikeCount] = useState(props.numLikes);
    const [commentCount, setCommentCount] = useState(props.numComments);
    const [poster, setPoster] = useState("");
    const [distBetween, setDistBetween] = useState(0);
    const [imageURI, setImageURI] = useState<string>();
    const [outOfRadius, setOutOfRadius] = useState(false);


    const router = useRouter();
    const params = useLocalSearchParams();
    const uid: string = auth.currentUser!.uid;

    const getPoster = async () => {
        const docSnap = await getDoc(doc(db, "user", props.uid));

        if (docSnap.exists()) {
            setPoster(docSnap.data().username)
            return docSnap.data().username;
        }
        else {
            setPoster("InvalidUser");
            return "InvalidUser";
        }
        
    }

    const getImage = async () => {
        if (props.image != "" && props.image != null) {
            const gsRef = ref(storage, "images/" + props.image);
            getDownloadURL(gsRef).then( (url) => {
                setImageURI(url);
                console.log(url);
            });
        } else{
            setImageURI("");
        } 
    }

    useEffect( () => {
        getPoster();
        getImage();
        checkForID(uid);
        (async () => {
            const distInBtwn = await getUserDistanceFromPost(props.location.latitude, props.location.longitude)
            setDistBetween(distInBtwn);
            console.log("Distance between user and post (km): " + distInBtwn);
            if (distInBtwn > 5) {
                console.log("Too far => Blur");
                setOutOfRadius(true);
            } else {
                setOutOfRadius(false);
            }
        })();
        
    }, []);


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
        fetchUser(props.uid);
    }, [props.uid])

    if(profilePictureID){
        getProfileImage(profilePictureID as string).then( async (uri) => {
          if (uri) {
              setpfpURI(uri);
          } else {
              setpfpURI("");
          }
     })};

     // This function toggles the like and unlike.
    const toggleLike = async () => {
        setIsLiked(!isLiked);
        const cacheRef = doc(db, "cache", props.id);
        if (isLiked) {
            setLikeCount(likeCount - 1)     // Decrement local like counter
            await updateDoc(cacheRef, {
                numLikes: increment(-1),    // Decrement cache like counter
                likeIDs: arrayRemove(uid),  // Remove user ID from caches likeID array
            });
        } else {
            setLikeCount(likeCount + 1)     // Increment local like counter
            await updateDoc(cacheRef, {
                numLikes: increment(1),     // Increment cache like counter
                likeIDs: arrayUnion(uid),   // Add user ID from caches likeID array
            });
        
        };
        
    };

    const viewPost = () => {
        const { postId = props.id } = params;
        router.push({ pathname: '/postPageEX', params: { postId }}); // userId, username, imageRef, caption, distBtwn, timePosted, location, nLikes, liked, nComments}  
    }

    const viewProfile = () => {
        if (props.uid == auth.currentUser?.uid) {
            router.push({ pathname: 'profile' })
            return;
        }
        const { userId = props.uid } = params;
        router.push({ pathname: '/viewProfile', params: { userId }});
    }
    // This function checks if the users ID is inside a caches likeIDs array. If it is, the post is set to liked.
    const checkForID = (stringToCheck: string) => {
        props.likeIDs.forEach((ID) => {
            if (ID === stringToCheck) {
                setIsLiked(!isLiked);
            }
        });
    };


    return (
        <View style={styles.outerContainer}>
            <View style={styles.container}>
            {/* Top bar with profile picture name and time uploaded */}
             <View style={styles.profileContainer}>
             {pfpURI != '' && 
                <Image source={{uri: pfpURI}} 
                style={{
                    width: windowWidth*.125,
                    height: windowWidth*.125,
                    borderRadius: (windowWidth*.125)/2,
                }}
             />} 
                
                <TouchableOpacity style={styles.profileContainer} onPress={viewProfile}>
                    <RegularText style={styles.name} >{ poster }</RegularText>
                </TouchableOpacity>
             </View>
             <Text style={styles.postTime}>{ moment(props.timePosted.toDate()).fromNow() }</Text>
            </View>
            {(props.image == null || props.image == "") ? (
                <></>
            ) : (<View style={styles.postContainer }> 
                    <Image source={{uri: imageURI}}
                    blurRadius={outOfRadius ? 20 : 0}
                    style={{
                        width: width - 30, 
                        height: width - 30, 
                        borderRadius: 15,
                        }}
                    />
                </View> 
            )}
            {/* Caption of post */}
            <View style={{paddingBottom: 5}} >
                <RegularText style={outOfRadius ? styles.blurredText : styles.text}>{props.captionText}</RegularText>
            </View>
            <View>
                {/* The bottom Green Blue bar and post interactables*/}
                <LinearGradient     
                    colors={['rgba(44, 218, 157, .45)', 'rgba(4, 67, 137, .45)']}
                    start={{ x: 0, y: 0 }}
                    end={{ x: 1, y: 1 }}
                    style={styles.gradient}>
                    <View style={styles.icons}>
                        <TouchableOpacity onPress={toggleLike}>
                            <AntDesign name="heart" size={35} color={isLiked ? '#8E4162' : '#FFFFFF'}/>
                        </TouchableOpacity>
                        <RegularText style={{ marginLeft: 5}}>{likeCount}</RegularText>

                        <TouchableOpacity onPress={viewPost}>
                            <FontAwesome name="comment" size={35} color="white" style={{marginLeft: 5}}/>
                        </TouchableOpacity>
                        <RegularText style={{ marginLeft: 5}}>{commentCount}</RegularText>
                    </View>
                    <View style={styles.icons}>
                        <RegularText>{distBetween} km away</RegularText>
                        <TouchableOpacity onPress={() => {
                            const {latitude = props.location.latitude, longitude = props.location.longitude} = params;
                            router.push({pathname: "/map", params: {latitude, longitude}});
                        }}>
                            <Ionicons name="location-sharp" size={35} color="white" />
                        </TouchableOpacity>
                    </View>
                </LinearGradient>
            </View>
        </View>
    )
}



const styles = StyleSheet.create({
    outerContainer: {
        transform: [{ scale: 0.97}],
        flex: 1,
        flexDirection: 'column',
        paddingBottom: 0,
        marginVertical: 0,
        margin: 5,
        marginBottom: 0,
        backgroundColor: "white",
        borderRadius: 15,
    },
    postContainer: {
        justifyContent: "space-between",
        alignItems: "center"
    }, 
    text: {
        paddingTop: 10,
        marginLeft: 20,
        marginRight: 20,
        justifyContent: "flex-start",
    },
    blurredText: {
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
    },
    gradient: {
        flex: 1,
        paddingTop: 10,
        paddingBottom: 10,
        borderBottomLeftRadius: 15,
        borderBottomRightRadius: 15,
        flexDirection: 'row',
        justifyContent: 'space-between',
        alignItems: 'center',
    },
    icons: {
        flexDirection: "row",
        marginHorizontal: 20,
    },
    container: {
        flexDirection: 'row',
        justifyContent: 'space-between',
        alignItems: 'center',
        margin: 10,
    },
    profileContainer: {
        flexDirection: 'row',
        alignItems: 'center',
        marginLeft: 8,
    },
    profileImage: {
        width: 50,
        height: 50,
        borderRadius: 25,
    },
    name: {
        fontWeight: "700",
        fontSize: 16,
    },
    postTime: {
        fontSize: 14,
        fontWeight: "500",
    },
    location: {
        fontSize: 14,
        fontWeight: "500",
    }
})


export default PostPreview

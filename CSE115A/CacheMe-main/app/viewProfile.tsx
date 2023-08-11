import { StyleSheet, Text, TouchableOpacity, View, ActivityIndicator, FlatList } from 'react-native'
import { auth, db } from '../firebase'
import { useRouter, useSearchParams } from 'expo-router';
import ProfilePicture from "../components/profile/ProfilePicture"
import { useEffect, useState } from 'react';
import { GeoPoint, Timestamp, arrayRemove, arrayUnion, collection, doc, getDoc, getDocs, orderBy, query, updateDoc, where } from 'firebase/firestore';
import { FontAwesome, Ionicons } from '@expo/vector-icons';
import PostPreview from '../components/Post';

type UserData = {
    __id: string;
    email: string;
    name: string;
    profilePicture: string;
    username: string;
    friends: [];
}

const cache = {
  __id: "",
  __imageId: "",
  __userId: "",
  _createdAt: new Timestamp(0, 0),
  caption: "",
  likeIDs: [],
  location: new GeoPoint(0, 0),
  numComments: 0,
  numLikes: 0,
  reported: false
}

type CacheData = {
  __id: string; 
  __imageId: string; 
  __userId: string; 
  _createdAt: Timestamp; 
  caption: string; 
  likeIDs: never[],
  location: GeoPoint; 
  numComments: number; 
  numLikes: number; 
  reported: boolean;
}

const ViewProfile = () => {

    const { userId } = useSearchParams();
    const uid = userId as string;
    
    const [user, setUser] =  useState<UserData>();
    const [profilePictureID, setProfilePictureID] = useState<string>();
    const [friended, setFriended] = useState(false);
    const router = useRouter();

    const [postsToRender, setPostsToRender] = useState([cache]);
    const [loading, setLoading] = useState(true);

    useEffect (() => {
        const fetchUserPosts = async (id: string) => {
          const cacheList:CacheData[] = [];
          const cacheRef = collection(db, "cache");
          const usersCacheQuery = query(cacheRef, where("__userId", "==", id), orderBy("_createdAt", "desc"));
          const querySnapshot = await getDocs(usersCacheQuery);

          querySnapshot.forEach((cache) => {
            const {
              __id,
              __imageId,
              __userId,
              _createdAt,
              caption,
              location,
              numComments,
              numLikes,
              reported,
              likeIDs,
            } = cache.data();

            cacheList.push({
              __id,
              __imageId,
              __userId,
              _createdAt,
              caption,
              location,
              numComments,
              numLikes,
              reported,
              likeIDs,
            });
          });

          setPostsToRender(cacheList);

          if (loading) {
            setLoading(false);
          }
        }
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

            const currUserDoc = await getDoc(doc(db, "user", auth.currentUser?.uid as string));
            if (currUserDoc.exists()) {
              let currUserFriendList = currUserDoc.data().friends;
              if (currUserFriendList.includes(user.__id)) {
                setFriended(true);
              }
              else {
                setFriended(false);
              }
            }
        }

        fetchUser(uid);  // grab uid of the user whose profile is being viewed and use it here
        fetchUserPosts(uid);
    }, [uid]) 

    const toggleAddFriend = () => {
      const currUserDoc = doc(db, "user", auth.currentUser?.uid as string);
      if (friended) {
        console.log("remove friend");
        updateDoc(currUserDoc, {
          friends: arrayRemove(user?.__id),
        });
        setFriended(false)
      } else {
        console.log("add friend")
        updateDoc(currUserDoc, {
          friends: arrayUnion(user?.__id),
        });
        setFriended(true)
      }
    }

    return (
        <><View style={styles.container}>
        <View style={styles.header}>
          <TouchableOpacity style={styles.backArrow} onPress={router.back}>
            <Ionicons name="arrow-back-outline" size={35} color="black" />
          </TouchableOpacity>
        </View>
        <View style={styles.profileContainer}>
          {profilePictureID && <ProfilePicture profilePictureID={profilePictureID} />}
          <Text style={styles.nameText}>{user?.name}</Text>
          <Text>@{user?.username}</Text>
          <View style={styles.profileData}>
            <View style={styles.data}>
              <Text style={styles.number}>{user?.friends.length}</Text>
              <Text>Friends</Text>
            </View>
            <View style={styles.data}>
              <Text style={styles.number}>{postsToRender.length}</Text>
              <Text>Posts</Text>
            </View>
          </View>
          {friended ? (
            <TouchableOpacity style={styles.friendButton} onPress={toggleAddFriend}>
              <FontAwesome name="check" size={15} style={{ paddingRight: 5 }} color="black" />
              <Text style={{ fontWeight: "500" }}>Added</Text>
            </TouchableOpacity>
          ) : (
            <TouchableOpacity style={styles.friendButton} onPress={toggleAddFriend}>
              <FontAwesome name="user-plus" size={15} style={{ paddingRight: 5 }} color="black" />
              <Text style={{ fontWeight: "500" }}>Friend</Text>
            </TouchableOpacity>
          )}
        </View>
      </View>
      <View style={styles.profileFeed}>
          {loading ? (
            <ActivityIndicator size="large" />
          ) : (
            <FlatList data={postsToRender}
              keyExtractor={(c) => c.__id}
              renderItem={({ item }) => {
                return (
                  <PostPreview id={item.__id}
                    captionText={item.caption}
                    uid={item.__userId}
                    image={item.__imageId}
                    numComments={item.numComments}
                    numLikes={item.numLikes}
                    location={item.location}
                    timePosted={item._createdAt}
                    likeIDs={item.likeIDs} />
                );
              } }
              showsVerticalScrollIndicator={false} />
          )}
      </View></>
    )

}

const styles = StyleSheet.create({
    container: {
      // flex: 1,
      justifyContent: 'flex-start',
      flexDirection: 'column',
      alignItems: 'center',
      flexWrap: 'wrap',
      paddingBottom: 20,
      backgroundColor: '#EEF2FF',
    },
    header: {
      // flex: 1,
      width: "100%",
      height: 50,
      flexDirection: 'row',
      justifyContent: 'space-between',
      marginTop: 30,
    },
    backArrow: {
      //flex: 1,
      width: 50,
      height: 30,
      marginLeft: 10,
      marginTop: 5,
    },
    profileContainer: {
      // flex: 5,
      width: '100%',
      height: 263,
      alignItems: 'center',
      flexWrap: 'wrap',
    },
    nameText: {
      fontSize: 20,
      fontWeight: "500",
    },
    profileData: {
      flex: 1,
      width: 164,
      height: 41,
      justifyContent: 'space-between',
      flexDirection: 'row',
    },
    number: {
      fontSize: 18,
      fontWeight: "500",
    },
    data: {
      flexDirection: 'column',
      justifyContent: 'center',
      alignItems: 'center',
    },
    friendButton: {
      width: 90,
      height: 30,
      flexDirection: "row",
      alignItems: 'center',
      textSize: 12,
      justifyContent: 'center',
      backgroundColor: 'white',
      borderRadius: 10,
    },
    signoutButton: {
      width: 90,
      height: 30,
      marginTop: 10,
      flexDirection: "row",
      alignItems: 'center',
      textSize: 12,
      justifyContent: 'center',
      backgroundColor: 'white',
      borderRadius: 10,
      marginRight: 10,
    },
    profileFeed: {
      flex: 1,
      backgroundColor: '#EEF2FF',
    },
  })

export default ViewProfile
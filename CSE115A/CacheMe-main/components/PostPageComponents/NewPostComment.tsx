import React, { useEffect, useState } from 'react';
import { Dimensions, View, StyleSheet, SafeAreaView, Image } from 'react-native';
import RegularText from "../Texts/regularText"
import { Timestamp, doc, getDoc } from 'firebase/firestore';
import { getPoster, getTimeDifference } from '../PostData';
import { db } from '../../firebase';

import getProfileImage from "../../helpers/profile";


type CommentProps = {
    id: string
}

type CommentData = {
    __id: string;
    __userId: string;
    _createdAt: Timestamp;
    username: string;
    replies: never[];
    text: string;
    timePosted: string;
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


const NewPostComment = (props: CommentProps) => {
    const [comment, setComment] = useState<CommentData>();
    const [pfpuserid, setpfpuserid] = useState<string>();
    const [pfpURI, setpfpURI] = useState("");

    const [user, setUser] =  useState<UserData>();
    const [profilePictureID, setProfilePictureID] = useState<string>();

    useEffect(() => {
        // This has to be in here, see: https://devtrium.com/posts/async-functions-useeffect
        const fetchComment = async (id: string) => {
            const commentsList:CommentData[] = [];
            const comment: CommentData = {
            __id:  "",
            __userId: "",
            _createdAt: new Timestamp(0, 0),
            username: "",
            replies: [],
            text: "",
            timePosted: "",
            }
            // For each comment ID, find the document for the comment
            const commentDoc = await getDoc(doc(db, "comment", id));
            if (commentDoc.exists()) {
            comment.__id = commentDoc.data().__id;
            comment.__userId = commentDoc.data().__userId;
            setpfpuserid(comment.__userId);
            comment.username = await getPoster(comment.__userId)
            // setCommenter(username);
            comment._createdAt = commentDoc.data()._createdAt;
            comment.timePosted = getTimeDifference(comment._createdAt)
            comment.replies = commentDoc.data().replies;
            comment.text = commentDoc.data().text;
            commentsList.push(comment);
            }
            setComment(comment);
        }
        fetchComment(props.id);

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
        
        
        if(pfpuserid){
            fetchUser(pfpuserid as string);
        }
      }, [pfpuserid])

    if(profilePictureID){
      getProfileImage(profilePictureID as string).then( async (uri) => {
        if (uri) {
            setpfpURI(uri);
        } else {
            setpfpURI("");
        }
   })};

      return (
        <>
        <SafeAreaView>
          <View style={styles.container}>
            <View style={styles.topBar}>
              <View>
              {pfpURI != '' && 
                <Image source={{uri: pfpURI}} 
                style={{
                    width: windowWidth*.1,
                    height: windowWidth*.1,
                    borderRadius: (windowWidth*.1)/2
                }}
             />}              
             </View>
              <View style={styles.userName}>
                <RegularText style={{fontWeight: "bold", fontSize: 14}}>{comment?.username}</RegularText>
              </View>
              <View>
                <RegularText style={styles.time}>{comment?.timePosted}</RegularText>
              </View>
            </View>
            <View style={styles.midderBar}>
              <RegularText style={{ fontSize: 15 }}>{comment?.text}</RegularText>
            </View>
       
            {/* <TouchableOpacity style={styles.bottomBar}>
              <RegularText style={{ fontSize: 13, marginTop: 10, }}>Reply</RegularText>
            </TouchableOpacity> */}
    
            {/* {showReplyBox && (
              <View style={styles.replyContainer}>
                <TextInput
                  style={styles.replyInput}
                  placeholder="Type your reply..."
                  value={replyMessage}
                  onChangeText={setReplyMessage} />
                <TouchableOpacity style={styles.replyButton} onPress={handleReply}>
                  <Text style={styles.replyButtonText}>Send</Text>
                </TouchableOpacity>
              </View>
            )}
    
    
              {/* Display the posted reply */}
              {/* postedReply !== '' && (
                <View style={styles.postedReplyContainer}>
                  <RegularText style={styles.postedReplyText}>{postedReply}</RegularText>
                </View>
               )} */}
    
            </View>
        </SafeAreaView></>
      );
    };
    
    const styles = StyleSheet.create({
      container: {
        flex: 1,
        backgroundColor: '#FFFFFF',
        marginVertical: 10,
        flexDirection: "column",
      },
      topBar: {
        outline: '#FF5733',
        flex: 1,
        flexDirection: "row",
        justifyContent: "space-between",
        alignItems: "center",
        paddingLeft: 5,
      },
      userName:{
        flex: 1,
        position: "absolute",
        marginLeft: 45,
      },
      time:{
        flex: 1,
        fontSize: 13,
        color: "#545350",
        paddingRight: 10,
      },
      midderBar: {
        flex: 1,
        paddingLeft: 45,
      },
     
      bottomBar: {
        paddingLeft: 45,
      },
      replyContainer: {
        flexDirection: 'row',
        alignItems: 'center',
        paddingHorizontal: 45,
        paddingTop: 10,
      },
      replyInput: {
        flex: 1,
        height: 40,
        borderWidth: 1,
        borderColor: '#000',
        borderRadius: 5,
        paddingHorizontal: 10,
        marginRight: 10,
      },
      replyButton: {
        backgroundColor: 'blue',
        paddingHorizontal: 15,
        paddingVertical: 10,
        borderRadius: 5,
      },
      replyButtonText: {
        color: '#FFF',
        fontSize: 14,
      },
      postedReplyContainer: {
        paddingHorizontal: 45,
        marginTop: 10,
      },
      postedReplyText: {
        fontSize: 15,
      },
    });

export default NewPostComment;
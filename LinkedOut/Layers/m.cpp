#pragma once
#include <string>
#include <vector>
#include <memory>


	class Time {
	public:
		Time() = default;

		Time(const std::string& day, const std::string& month, const std::string& year, const std::string& hour, const std::string& minute, const std::string& second)
			: m_Day(day), m_Month(month), m_Year(year), m_Hour(hour), m_Minute(minute), m_Second(second)
		{
		}


		const std::string& GetDay()const { return m_Day; }
		const std::string& GetMonth()const { return m_Month; }
		const std::string& GetYear()const { return m_Year; }
		const std::string& GetHour()const { return m_Hour; }
		const std::string& GetMinute()const { return m_Minute; }
		const std::string& GetSecond()const { return m_Second; }

		void SetDay(const std::string& Day) { m_Day = Day; }
		void SetMonth(const std::string& Month) { m_Month = Month; }
		void SetYear(const std::string& Year) { m_Year = Year; }
		void SetHour(const std::string& Hour) { m_Hour = Hour; }
		void SetMinute(const std::string& Minute) { m_Minute = Minute; }
		void SetSecond(const std::string& Second) { m_Second = Second; }

	private:
		std::string m_Day = "0";
		std::string m_Month = "0";
		std::string m_Year = "0";
		std::string m_Hour = "0";
		std::string m_Minute = "0";
		std::string m_Second = "0";
	};

	class Content {
	public:
		Content() = default;

		Content(const std::string& SenderID, const Time& TimeSent, const std::string& ContentText, const std::string& ContentPicture, const std::string& ContentVideo)
			: m_SenderID(SenderID), m_TimeSent(TimeSent), m_ContentText(ContentText), m_ContentPicture(ContentPicture), m_ContentVideo(ContentVideo)
		{
		}
		const std::string& GetSenderID() const { return m_SenderID; }
		const Time& GetTimeSent() const { return m_TimeSent; }
		const std::string& GetContentText() const { return m_ContentText; }
		const std::string& GetContentPicture() const { return m_ContentPicture; }
		const std::string& GetContentVideo() const { return m_ContentVideo; }


		void SetSenderID(const std::string& SenderID) { m_SenderID = SenderID; }
		void SetTimeSent(const Time& TimeSent) { m_TimeSent = TimeSent; }
		void SetContentText(const std::string& ContentText) { m_ContentText = ContentText; }
		void SetContentPicture(const std::string& ContentPicture) { m_ContentPicture = ContentPicture; }
		void SetContentVideo(const std::string& ContentVideo) { m_ContentVideo = ContentVideo; }

	protected:
		std::string m_SenderID;
		Time m_TimeSent;
		std::string m_ContentText;
		std::string m_ContentPicture;
		std::string m_ContentVideo;
	};
	class Like {
	public:
		Like() = default;

		const std::string& GetLikedBy()const { return m_LikedBy; }
		const std::string& GetLikeID()const { return m_LikeID; }
		const Time& GetLikedAt()const { return m_LikedAt; }

		void SetLikedBy(const std::string& likedBy) { m_LikedBy = likedBy; }
		void SetLikeID(const std::string& likeID) { m_LikeID = likeID; }
		void SetLikedAt(const Time& time) { m_LikedAt = time; }

	private:
		std::string m_LikedBy;
		std::string m_LikeID;
		Time m_LikedAt;
	};
	class Comment : public Content {
	public:
		Comment() = default;

		const std::string& GetPostID()const { return m_PostID; }
		const std::string& GetCommentID()const { return m_CommentID; }
		const std::string& GetCommenterName()const { return m_CommenterName; }

		void SetPostID(const std::string& postID) { m_PostID = postID; }
		void SetCommentID(const std::string& commentID) { m_CommentID = commentID; }
		void SetCommenterName(const std::string& name) { m_CommenterName = name; }


	private:
		std::string m_PostID;
		std::string m_CommentID;
		std::string m_CommenterName;
	};
	class Post : public Content {
	public:
		Post() = default;

		const std::string& GetPostID()const { return m_PostID; }
		const uint32_t& GetRepostCounter()const { return m_RepostCounter; }
		const std::vector<Like>& GetLikes()const { return m_Likes; }
		const std::vector<std::shared_ptr<Comment>>& GetComments()const { return m_Comments; }
		const std::string& GetPosterName()const { return m_PosterName; }

		void SetPostID(const std::string& PostID) { m_PostID = PostID; }
		void SetRepostCounter(const uint32_t& RepostCounter) { m_RepostCounter = RepostCounter; }
		void SetLikes(const std::vector<Like>& Likes) { m_Likes = Likes; }
		void SetComments(const std::vector<std::shared_ptr<Comment>>& Comments) { m_Comments = Comments; }
		void SetPosterName(const std::string& name) { m_PosterName = name; }

	private:
		std::string m_PosterName;
		std::string m_PostID;
		uint32_t m_RepostCounter;
		std::vector<Like> m_Likes;
		std::vector<std::shared_ptr<Comment>> m_Comments;
	};

	class DirectMessage : public Content {
	public:
		DirectMessage() = default;

		const std::string& GetMessageID()const { return m_MessageID; }
		const std::string& GetToWho()const { return m_ToWho; }

		void SetMessageID(const std::string& messageID) { m_MessageID = messageID; }
		void SetToWho(const std::string& toWho) { m_ToWho = toWho; }
	private:
		std::string m_MessageID;
		std::string m_ToWho;
	};

	class Account {
	public:

		Account() = default;

		std::string GetAccountID() const {
			return m_AccountID;
		}

		void SetAccountID(const std::string& accountID) {
			m_AccountID = accountID;
		}

		std::string GetPhoneNumber() const {
			return m_PhoneNumber;
		}

		void SetPhoneNumber(const std::string& phoneNumber) {
			m_PhoneNumber = phoneNumber;
		}

		std::string GetEmail() const {
			return Email;
		}

		void SetEmail(const std::string& email) {
			Email = email;
		}

		std::vector<std::string> GetFollowingID() const {
			return m_FollowingID;
		}

		void SetFollowingID(const std::vector<std::string>& followingID) {
			m_FollowingID = followingID;
		}

		std::vector<std::shared_ptr<Post>> GetPosts() const {
			return m_Posts;
		}

		void SetPosts(const std::vector<std::shared_ptr<Post>>& posts) {
			m_Posts = posts;
		}

		std::vector<std::shared_ptr<DirectMessage>> GetDirectMessages() const {
			return m_DirectMessages;
		}

		void SetDirectMessages(const std::vector<std::shared_ptr<DirectMessage>>& directMessages) {
			m_DirectMessages = directMessages;
		}

	private:
		std::string m_AccountID;
		std::string m_PhoneNumber;
		std::string Email;
		std::vector<std::string> m_FollowingID;
		std::vector<std::shared_ptr<Post>> m_Posts;
		std::vector<std::shared_ptr<DirectMessage>> m_DirectMessages;
	};

	enum class SkillAmount {
		None,
		Beginner,
		Intermediate,
		Advanced,
	};



	struct Skill {
		std::string Name;
		SkillAmount SkillAmount;
	};

	enum class WorkspaceType {
		None,
		OnSite,
		Remote,
		Hybrid
	};

	enum class JobType {
		None,
		FullTime,
		PartTime,
		Contract,
		Temporary,
		Other,
		Volunteer,
		Internship
	};


	class Job {
	public:

		uint64_t GetSalary() const {
			return m_Salary;
		}

		void SetSalary(uint64_t salary) {
			m_Salary = salary;
		}

		const std::string& GetJobName() const {
			return m_JobName;
		}

		void SetJobName(const std::string& jobName) {
			m_JobName = jobName;
		}

		const std::string& GetCompanyName() const {
			return m_CompanyName;
		}

		void SetCompanyName(const std::string& companyName) {
			m_CompanyName = companyName;
		}

		const std::vector<Skill>& GetRequiredSkills() const {
			return m_RequiredSkills;
		}

		void SetRequiredSkills(const std::vector<Skill>& requiredSkills) {
			m_RequiredSkills = requiredSkills;
		}

		WorkspaceType GetWorkspaceType() const {
			return m_WorkspaceType;
		}

		void SetWorkspaceType(WorkspaceType workspaceType) {
			m_WorkspaceType = workspaceType;
		}

		const std::string& GetLocation() const {
			return m_Location;
		}

		void SetLocation(const std::string& location) {
			m_Location = location;
		}

		JobType GetJobType() const {
			return m_JobType;
		}

		void SetJobType(JobType jobType) {
			m_JobType = jobType;
		}

	private:
		uint64_t m_Salary;
		std::string m_JobName;
		std::string m_CompanyName;
		std::vector<Skill> m_RequiredSkills;
		WorkspaceType m_WorkspaceType;
		std::string m_Location;
		JobType m_JobType;
	};

	class Person : public Account {
	public:

		void TakeJob(std::shared_ptr<Job> job) {
			//TODO:take job.
		}

	private:
		std::string m_FirstName;
		std::string m_LastName;
		std::vector<Skill> m_Skills;
	};
	class Company : public Account {
	public:
		std::shared_ptr<Job> CreateJob() { m_PostedJobs.push_back(std::make_shared<Job>()); return m_PostedJobs.back(); }


		const std::vector<std::shared_ptr<Job>>& GetPostedJobs() const {
			return m_PostedJobs;
		}

		void SetPostedJobs(const std::vector<std::shared_ptr<Job>>& postedJobs) {
			m_PostedJobs = postedJobs;
		}

		const std::string& GetCompanyName() const {
			return m_CompanyName;
		}

		void SetCompanyName(const std::string& companyName) {
			m_CompanyName = companyName;
		}

		const std::vector<std::shared_ptr<Person>>& GetEmployees() const {
			return m_Employees;
		}

		void SetEmployees(const std::vector<std::shared_ptr<Person>>& employees) {
			m_Employees = employees;
		}

	private:
		std::vector<std::shared_ptr<Job>> m_PostedJobs;
		std::string m_CompanyName;
		std::vector<std::shared_ptr<Person>> m_Employees;
	};

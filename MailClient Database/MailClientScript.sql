SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[User]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[User](
	[UserId] [int] IDENTITY(1,1) NOT NULL,
	[Username] [nvarchar](50) NULL,
	[Password] [nvarchar](50) NULL,
	[EmailAddress] [nvarchar](1024) NULL,
	[DisplayName] [nvarchar](1024) NULL,
 CONSTRAINT [PK_User] PRIMARY KEY CLUSTERED 
(
	[UserId] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY],
 CONSTRAINT [IX_User] UNIQUE NONCLUSTERED 
(
	[Username] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[Group]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[Group](
	[GroupId] [int] IDENTITY(1,1) NOT NULL,
	[GroupName] [nvarchar](1024) NULL,
	[Description] [nvarchar](2048) NULL,
 CONSTRAINT [PK_Group] PRIMARY KEY CLUSTERED 
(
	[GroupId] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[AddressBook]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[AddressBook](
	[UserId] [int] NOT NULL,
	[FriendId] [int] NOT NULL,
	[Description] [nvarchar](1024) NULL,
 CONSTRAINT [PK_AddressBook] PRIMARY KEY CLUSTERED 
(
	[UserId] ASC,
	[FriendId] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[MailHeader]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[MailHeader](
	[MessageID] [nvarchar](1024) NULL,
	[From] [nvarchar](1024) NULL,
	[To] [nvarchar](1024) NULL,
	[Date] [datetime] NULL,
	[Subject] [nvarchar](1024) NULL,
	[Cc] [nvarchar](1024) NULL,
	[ReplyTo] [nvarchar](1024) NULL,
	[TextBody] [nvarchar](2048) NULL,
	[MimeVersion] [nvarchar](1024) NULL,
	[ContentType] [nvarchar](1024) NULL,
	[RealAttach] [int] NULL,
	[UserId] [int] NULL,
	[MailId] [int] IDENTITY(1,1) NOT NULL,
	[GroupId] [int] NULL,
 CONSTRAINT [PK_MailHeader] PRIMARY KEY CLUSTERED 
(
	[MailId] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
IF NOT EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_AddressBook_User]') AND parent_object_id = OBJECT_ID(N'[dbo].[AddressBook]'))
ALTER TABLE [dbo].[AddressBook]  WITH CHECK ADD  CONSTRAINT [FK_AddressBook_User] FOREIGN KEY([UserId])
REFERENCES [dbo].[User] ([UserId])
GO
IF NOT EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_AddressBook_User1]') AND parent_object_id = OBJECT_ID(N'[dbo].[AddressBook]'))
ALTER TABLE [dbo].[AddressBook]  WITH CHECK ADD  CONSTRAINT [FK_AddressBook_User1] FOREIGN KEY([FriendId])
REFERENCES [dbo].[User] ([UserId])
GO
IF NOT EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_MailHeader_Group]') AND parent_object_id = OBJECT_ID(N'[dbo].[MailHeader]'))
ALTER TABLE [dbo].[MailHeader]  WITH CHECK ADD  CONSTRAINT [FK_MailHeader_Group] FOREIGN KEY([GroupId])
REFERENCES [dbo].[Group] ([GroupId])
GO
IF NOT EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_MailHeader_User]') AND parent_object_id = OBJECT_ID(N'[dbo].[MailHeader]'))
ALTER TABLE [dbo].[MailHeader]  WITH CHECK ADD  CONSTRAINT [FK_MailHeader_User] FOREIGN KEY([UserId])
REFERENCES [dbo].[User] ([UserId])
